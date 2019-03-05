//// Main.cpp
//
//#include <SDL.h>
//#include <cstdlib>
//
//int SDL_main(int argc, char *args[]) {
//	// ##### FIXME! #####
//
//	return EXIT_SUCCESS;
//}
#include "pch.h"
#define DO_TRI 1

// Basic OpenGL ES 3 + SDL2 template code
#include <SDL.h>
#include <SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>

const unsigned int DISP_WIDTH = 800;
const unsigned int DISP_HEIGHT = 640;
/** Encapsulates the data for a single vertex.
* Must match the vertex shader's input.
*/
typedef struct Vertex_s {
	float position[2];
} Vertex;


/** Gets the file's length.
*
* @param file the file
*
* @return size_t the file's length in bytes
*/
static size_t fileGetLength(FILE *file) {
	size_t length;
	size_t currPos = ftell(file);
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	// Return the file to its previous position
	fseek(file, currPos, SEEK_SET);
	return length;
}

/** Loads and compiles a shader from a file.
*
* This will print any errors to the console.
*
* @param filename the shader's filename
* @param shaderType the shader type (e.g., GL_VERTEX_SHADER)
*
* @return GLuint the shader's ID, or 0 if failed
*/
static GLuint shaderLoad(const char *filename, GLenum shaderType) {
	errno_t err;
	FILE *file;
	err = fopen_s(&file,filename, "r");
	if (err != 0) {
		SDL_Log("Can't open file: %s\n", filename);
		return 0;
	}
	size_t length = fileGetLength(file);
	// Alloc space for the file (plus '\0' termination)
	GLchar *shaderSrc = (GLchar*)calloc(length + 1, 1);
	if (!shaderSrc) {
		SDL_Log("Out of memory when reading file: %s\n", filename);
		fclose(file);
		file = NULL;
		return 0;
	}
	fread(shaderSrc, 1, length, file);
	// Done with the file
	fclose(file);
	file = NULL;
	// Create the shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, (const GLchar**)&shaderSrc, NULL);
	free(shaderSrc);
	shaderSrc = NULL;
	// Compile it
	glCompileShader(shader);
	GLint compileSucceeded = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);
	if (!compileSucceeded) {
		// Compilation failed. Print error info
		SDL_Log("Compilation of shader %s failed:\n", filename);
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar *errLog = (GLchar*)malloc(logLength);
		if (errLog) {
			glGetShaderInfoLog(shader, logLength, &logLength, errLog);
			SDL_Log("%s\n", errLog);
			free(errLog);
		}
		else {
			SDL_Log("Couldn't get shader log; out of memory\n");
		}
		glDeleteShader(shader);
		shader = 0;
	}
	return shader;
}
/** Destroys a shader.
*/
static void shaderDestroy(GLuint shaderID) {
	glDeleteShader(shaderID);
}
GLuint shaderProgLoad(const char *vertFilename, const char *fragFilename) {
	GLuint vertShader = shaderLoad(vertFilename, GL_VERTEX_SHADER);
	if (!vertShader) {
		SDL_Log("Couldn't load vertex shader: %s\n", vertFilename);
		return 0;
	}
	GLuint fragShader = shaderLoad(fragFilename, GL_FRAGMENT_SHADER);
	if (!fragShader) {
		SDL_Log("Couldn't load fragment shader: %s\n", fragFilename);
		shaderDestroy(vertShader);
		vertShader = 0;
		return 0;
	}
	GLuint shaderProg = glCreateProgram();
	if (shaderProg) {
		glAttachShader(shaderProg, vertShader);
		glAttachShader(shaderProg, fragShader);
		glLinkProgram(shaderProg);
		GLint linkingSucceeded = GL_FALSE;
		glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkingSucceeded);
		if (!linkingSucceeded) {
			SDL_Log("Linking shader failed (vert. shader: %s, frag. shader: %s\n",
				vertFilename, fragFilename);
			GLint logLength = 0;
			glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &logLength);
			GLchar *errLog = (GLchar*)malloc(logLength);
			if (errLog) {
				glGetProgramInfoLog(shaderProg, logLength, &logLength, errLog);
					SDL_Log("%s\n", errLog);
				free(errLog);
			}
			else {
				SDL_Log("Couldn't get shader link log; out of memory\n");
			}
			glDeleteProgram(shaderProg);
			shaderProg = 0;
		}
	}
	else {
		SDL_Log("Couldn't create shader program\n");
	}
	// Don't need these any more
	shaderDestroy(vertShader);
	shaderDestroy(fragShader);
	return shaderProg;
}

void shaderProgDestroy(GLuint shaderProg) {
	glDeleteProgram(shaderProg);
}

/** Creates the Vertex Buffer Object (VBO) containing
* the given vertices.
*
* @param vertices pointer to the array of vertices
* @param numVertices the number of vertices in the array
*/
GLuint vboCreate(const Vertex *vertices, GLuint numVertices) {
	// Create the Vertex Buffer Object
	GLuint vbo;
	int nBuffers = 1;
	glGenBuffers(nBuffers, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Copy the vertex data in, and deactivate
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertices,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Check for problems
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		// Failed
		glDeleteBuffers(nBuffers, &vbo);
		SDL_Log("Creating VBO failed, code %u\n", err);
		vbo = 0;
	}
	return vbo;
}
/** Frees the VBO.
*
* @param vbo the VBO's name.
*/
void vboFree(GLuint vbo) {
	glDeleteBuffers(1, &vbo);
}

#if USE_SDL2_TT_TRIAGNLE
int SDL_main(int argc, char *args[])
#else
int AAASDL_main(int argc, char *args[])
#endif
{
	// The window
	SDL_Window *window = NULL;
	// The OpenGL context
	SDL_GLContext context = NULL;
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	// Setup the exit hook
	atexit(SDL_Quit);
	//http://mickcharlesbeaver.blogspot.com/2017/11/using-opengl-es-20-with-sdl2-via-angle.html
//When trying to use OpenGL ES 2.0 from SDL2 on MS Windows, these lines are the magic ones :
	SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
	SDL_SetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER, "none");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	// Request OpenGL ES 3.0
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	// Want double-buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);



	//With base initialization done, we can now open the window and set up the OpenGL context:
	// Create the window
	window = SDL_CreateWindow("GLES3+SDL2 Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, DISP_WIDTH, DISP_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create the main window.", NULL);
		return EXIT_FAILURE;
	}
	context = SDL_GL_CreateContext(window);
	if (!context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create an OpenGL context.", NULL);
		return EXIT_FAILURE;
	}
	// Clear to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//GL_COLOR_BUFFER_BIT tells glClear() to only clear the screen/image (a.k.a., colour buffer).
	glClear(GL_COLOR_BUFFER_BIT);
	// Update the window
	SDL_GL_SwapWindow(window);

#if DO_TRI
	// Load the shader program and set it for use
	GLuint shaderProg = shaderProgLoad("Simple2D.vert", "Simple2D.frag");
	if (!shaderProg) {
		// Error messages already displayed...
		return EXIT_FAILURE;
	}
	glUseProgram(shaderProg);

	// Create the triangle
	const Vertex vertices[] = {
	{ 0.0f, -0.9f },
	{ 0.9f, 0.9f },
	{-0.9f, 0.9f } };
	GLsizei vertSize = sizeof(vertices[0]);
	GLsizei numVertices = sizeof(vertices) / vertSize;
	GLuint triangleVBO = vboCreate(vertices, numVertices);
	if (!triangleVBO) {
		// Failed. Error message has already been printed, so just quit
		return EXIT_FAILURE;
	}
	// Set up for rendering the triangle (activate the VBO)
	GLuint positionIdx = 0; // Position is vertex attribute 0
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glVertexAttribPointer(positionIdx, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (const GLvoid*)0);
	glEnableVertexAttribArray(positionIdx);


	//// Now draw!
	//glDrawArrays(GL_TRIANGLES, 0, numVertices);

	//// Update the window
	//SDL_GL_SwapWindow(window);
#endif


	/*
	Normally a ¡°real¡± OpenGL program would have a main loop that does things like respond to events
and render animated graphics. Since this is a really basic program, all that¡¯s needed is to wait for the
user to click the window¡¯s close button. SDL makes this relatively easy with its event handling
functions:
	*/
	// Wait for the user to quit
	bool quit = false;
	while (!quit) {
		SDL_Event event;
		/*
		SDL_WaitEvent() stops the program until an event comes in. If the incoming event is an SDL_QUIT,
then the code above exits the while loop, and quits.
		*/
		if (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT) {
				// User wants to quit
				quit = true;
				break;
			}
		}
#if 0
		//äÖÈ¾ÎªºìÉ«µÄ
		glClearColor(1, 0, 0, 1);
		//black
//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
#else
		//// Now draw!
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

//// Update the window
    SDL_GL_SwapWindow(window);
#endif
	}

#if DO_TRI
	// Cleanup
	vboFree(triangleVBO);
	triangleVBO = 0;
	shaderProgDestroy(shaderProg);
	shaderProg = 0;
#endif
	return EXIT_SUCCESS;
}