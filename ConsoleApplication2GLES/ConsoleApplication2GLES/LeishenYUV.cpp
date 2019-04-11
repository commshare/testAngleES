#include"pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <iostream>
// Basic OpenGL ES 3 + SDL2 template code
#include <SDL.h>
#include <SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include "draw_rgb.h"
SDL_GLContext yuvcontext = NULL;
SDL_Window *yuvwindow = NULL;
//Select one of the Texture mode (Set '1'):
#define TEXTURE_DEFAULT   1
//Rotate the texture
#define TEXTURE_ROTATE    0
//Show half of the Texture
#define TEXTURE_HALF      0
#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)

const int screen_w = 500, screen_h = 500;
const int ypixel_w = 320, ypixel_h = 180;
//YUV file
FILE *infile = NULL;
unsigned char buf[ypixel_w*ypixel_h * 3 / 2];
unsigned char *plane[3];

GLuint p;
GLuint id_y, id_u, id_v; // Texture id
GLuint textureUniformY, textureUniformU, textureUniformV;
//https://github.com/zcgit/cocos2dx-ffmpeg/blob/86d197fe9ba44de66637de298f71bb5f8cdfcc7b/video/FFSprite.cpp

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4


GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		//case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		//case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		//std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		delog("glerr[%d] %s %s\n",line,error.c_str(),file);
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 


void showYUV()
{
	//从infile读取一帧yuv到buf里
	if (fread(buf, 1, ypixel_w*ypixel_h * 3 / 2, infile) != ypixel_w * ypixel_h * 3 / 2)
	{
		// Loop
		fseek(infile, 0, SEEK_SET);
		fread(buf, 1, ypixel_w*ypixel_h * 3 / 2, infile);
		fread(buf, 1, ypixel_w*ypixel_h * 3 / 2, infile);
	}
	Assert(SDL_GL_MakeCurrent(yuvwindow, yuvcontext) == 0);

	//Clear
	glClearColor(0.0, 255, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Y
  //
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id_y);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ypixel_w, ypixel_h, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);
	glUniform1i(textureUniformY, 0);

	//U
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, id_u);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ypixel_w / 2, ypixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
	glUniform1i(textureUniformU, 1);

	//V
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, id_v);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ypixel_w / 2, ypixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
	glUniform1i(textureUniformV, 2);
	//GLuint err1 = glGetError();
	//if (err1 != GL_NO_ERROR) {
	//	//2017年5月6日 - GL_INVALID_OPERATION ：（1282）命令的状态集合对于指定的参数非法。
	//	delog("3333 Error OpenGL error setting up rendering %i\n", (unsigned int)err1);
	//}
	glCheckError();
	// Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	SDL_GL_SwapWindow(yuvwindow);
	//GLuint err = glGetError();
	//if (err != GL_NO_ERROR) {
	//	//2017年5月6日 - GL_INVALID_OPERATION ：（1282）命令的状态集合对于指定的参数非法。
	//	delog("22222 Error OpenGL error setting up rendering %i\n", (unsigned int)err);
	//}
	glCheckError();
}

char *textFileRead(char * filename)
{
	errno_t err;
	FILE * infile;
	char *s = (char *)malloc(8000);
	memset(s, 0, 8000);
	err = fopen_s(&infile, filename, "rb");
	if (err != 0)
	{
		std::cout << "open file fail " << std::endl;;
	}
	else
	{
		std::cout << "The file " << filename << " was opened\n" << std::endl;
	}
	int len = fread(s, 1, 8000, infile);
	fclose(infile);
	s[len] = 0;
	return s;
}
/*
所有的返回值有：
GL_NO_ERROR ：（0）当前无错误值
GL_INVALID_ENUM ：（1280）仅当使用非法枚举参数时，如果使用该参数有指定环境，则返回 GL_INVALID_OPERATION
GL_INVALID_VALUE ：（1281）仅当使用非法值参数时，如果使用该参数有指定环境，则返回 GL_INVALID_OPERATION
GL_INVALID_OPERATION ：（1282）命令的状态集合对于指定的参数非法。
GL_STACK_OVERFLOW ：（1283）压栈操作超出堆栈大小。
GL_STACK_UNDERFLOW ：（1284）出栈操作达到堆栈底部。
GL_OUT_OF_MEMORY ：（1285）不能分配足够内存时。
GL_INVALID_FRAMEBUFFER_OPERATION ：（1286）当操作未准备好的真缓存时。
GL_CONTEXT_LOST ：（1287）由于显卡重置导致 OpenGL context 丢失。
*/
/** Gets the file's length.
*
* @param file the file
*
* @return size_t the file's length in bytes
*/
static size_t AfileGetLength(FILE *file) {
	size_t length;
	size_t currPos = ftell(file);
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	// Return the file to its previous position
	fseek(file, currPos, SEEK_SET);
	return length;
}
/* http://www.ayqy.net/blog/uniform%E5%8F%98%E9%87%8F%E4%B8%8E%E7%89%87%E5%85%83%E7%9D%80%E8%89%B2%E5%99%A8_webgl%E7%AC%94%E8%AE%B04/
INFO: Compilation of shader Shader.fsh failed:
INFO: ERROR: 0:1: '' : No precision specified for (float)
ERROR: 0:7: '' : No precision specified for (float)
ERROR: 0:8: '' : No precision specified for (float)

INFO: Couldn't load fragment shader: Shader.fsh
shaderProgLoad fail
*/
/** Loads and compiles a shader from a file.
*
* This will print any errors to the console.
*
* @param filename the shader's filename
* @param shaderType the shader type (e.g., GL_VERTEX_SHADER)
*
* @return GLuint the shader's ID, or 0 if failed
*/
static GLuint AshaderLoad(const char *filename, GLenum shaderType) {
	errno_t err;
	FILE *file;
	err = fopen_s(&file, filename, "r");
	if (err != 0) {
		SDL_Log("Can't open file: %s\n", filename);
		return 0;
	}
	size_t length = AfileGetLength(file);
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
static void AshaderDestroy(GLuint shaderID) {
	glDeleteShader(shaderID);
}
GLuint AshaderProgLoad(const char *vertFilename, const char *fragFilename) {
	GLuint vertShader = AshaderLoad(vertFilename, GL_VERTEX_SHADER);
	if (!vertShader) {
		SDL_Log("Couldn't load vertex shader: %s\n", vertFilename);
		return 0;
	}
	GLuint fragShader = AshaderLoad(fragFilename, GL_FRAGMENT_SHADER);
	if (!fragShader) {
		SDL_Log("Couldn't load fragment shader: %s\n", fragFilename);
		AshaderDestroy(vertShader);
		vertShader = 0;
		return 0;
	}
	GLuint shaderProg = glCreateProgram();
	if (shaderProg)
	{
		glAttachShader(shaderProg, vertShader);
		glAttachShader(shaderProg, fragShader);
		glLinkProgram(shaderProg);
		GLint linkingSucceeded = GL_FALSE;
		glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkingSucceeded);
		if (!linkingSucceeded)
		{
			SDL_Log("Linking shader failed (vert. shader: %s, frag. shader: %s\n",
				vertFilename, fragFilename);
			GLint logLength = 0;
			glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &logLength);
			GLchar *errLog = (GLchar*)malloc(logLength);
			if (errLog)
			{
				glGetProgramInfoLog(shaderProg, logLength, &logLength, errLog);
				SDL_Log("%s\n", errLog);
				free(errLog);
			}
			else {
				SDL_Log("Couldn't get shader link log; out of memory\n");
			}
			//删除程序
			glDeleteProgram(shaderProg);
			shaderProg = 0;
		}
	}
	else {
		SDL_Log("Couldn't create shader program\n");
	}
	// Don't need these any more
	AshaderDestroy(vertShader);
	AshaderDestroy(fragShader);
	return shaderProg;
}

//Init Shader
void LOADSHaders()
{
	GLuint shaderProg;

#if 1
	GLint vertCompiled, fragCompiled, linked;

	GLint v, f;
	const char *vs, *fs;
	//Shader: step1
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	//Get source code
	char  vsh[] = "Shader.vsh";
	char  fsh[] = "Shader.fsh";
	vs = textFileRead(vsh);
	fs = textFileRead(fsh);
	//Shader: step2
	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	glCheckError();
	//Shader: step3
	glCompileShader(v);
	glCheckError();
	//Debug https://blog.csdn.net/panda1234lee/article/details/54287825
	glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != GL_TRUE)
	{
		delog("Vertex Shader compied error! \n");
	}
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != GL_TRUE)
	{
		delog("Fragment Shader compied error! \n");
	}
	glCheckError();

	//Program: Step1
	shaderProg = glCreateProgram();
	glCheckError();

	//Program: Step2
	glAttachShader(shaderProg, v);
	glAttachShader(shaderProg, f);
	glCheckError();

	glBindAttribLocation(shaderProg, ATTRIB_VERTEX, "vertexIn");
	glBindAttribLocation(shaderProg, ATTRIB_TEXTURE, "textureIn");
	glCheckError();

	//Program: Step3
	glLinkProgram(shaderProg);
	glCheckError();
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &linked); // Debug
	if (linked != GL_TRUE)
	{
		delog("Program linked error! \n");
	}
	//Program: Step4
	glUseProgram(shaderProg);
	glCheckError();
#else
	// Load the shader program and set it for use 加载shader
	shaderProg = AshaderProgLoad("Shader.vsh", "Shader.fsh");
	if (!shaderProg) {
		// Error messages already displayed...
		delog("shaderProgLoad fail \n");
		return;// EXIT_FAILURE;
	}
	glUseProgram(shaderProg);

#endif


	//GLuint err2 = glGetError();
	//if (err2 != GL_NO_ERROR)
	//{
	//	//2017年5月6日 - GL_INVALID_OPERATION ：（1282）命令的状态集合对于指定的参数非法。
	//	delog("4444 Error OpenGL error setting up rendering %i\n", (unsigned int)err2);
	//}
	
	//Get Uniform Variables Location
	textureUniformY = glGetUniformLocation(shaderProg, "tex_y");
	textureUniformU = glGetUniformLocation(shaderProg, "tex_u");
	textureUniformV = glGetUniformLocation(shaderProg, "tex_v");
	glCheckError();
#if TEXTURE_ROTATE
	static const GLfloat vertexVertices[] = {
			-1.0f, -0.5f,
			 0.5f, -1.0f,
			-0.5f,  1.0f,
			 1.0f,  0.5f,
	};
#else
	static const GLfloat vertexVertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
	};
#endif

#if TEXTURE_HALF
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		0.5f,  1.0f,
		0.0f,  0.0f,
		0.5f,  0.0f,
	};
#else
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,
	};
#endif

	//顶点坐标的数组
	//Set Arrays
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
	//Enable it
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
	glEnableVertexAttribArray(ATTRIB_TEXTURE);

	glBindTexture(GL_TEXTURE_2D,0);
	//YUV文理句柄
	glGenTextures(1, &id_y);
	glBindTexture(GL_TEXTURE_2D, id_y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &id_u);
	glBindTexture(GL_TEXTURE_2D, id_u);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &id_v);
	glBindTexture(GL_TEXTURE_2D, id_v);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//GLuint err = glGetError();
	//if (err != GL_NO_ERROR) {
	//	//2017年5月6日 - GL_INVALID_OPERATION ：（1282）命令的状态集合对于指定的参数非法。
	//	delog("11111 Error OpenGL error setting up rendering %i\n", (unsigned int)err);
	//}
	glCheckError();
}

int openfile()
{
	//Open YUV420P file
	errno_t err;
	if ((err = fopen_s(&infile, "./test_yuv420p_320x180.yuv", "rb") != 0)) {
		printf("cannot open this file\n");
		return -1;
	}
	//全局变量plane
	//YUV Data
	plane[0] = buf;
	plane[1] = plane[0] + ypixel_w * ypixel_h;
	plane[2] = plane[1] + ypixel_w * ypixel_h / 4;
#if 0
	uint8_t *luma = plane[0];
	uint8_t * cb = plane[1];
	uint8_t * cr = plane[2];
	const uint8_t *pixels[] = { luma, cb, cr };
	int frameWidth = 320;
	int frameHeight = 180;
	const int widths[] = { frameWidth, frameWidth / 2, frameWidth / 2 };
	const int heights[] = { frameHeight, frameHeight / 2, frameHeight / 2 };
	GLuint textures[3];
	for (int i = 0; i < 3; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_LUMINANCE,
			widths[i],
			heights[i],
			0,
			GL_LUMINANCE,
			GL_UNSIGNED_BYTE,
			pixels[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
#endif
	return 0;
}

int ttttt()
{
	static bool binit = false;

	//SDL_GLContext context;
	if (!binit)
	{
		//With base initialization done, we can now open the window and set up the OpenGL context:
// Create the window
		rgbwindow = SDL_CreateWindow("rgbwindow", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (!rgbwindow)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
				"Couldn't create the main window.", NULL);
			return EXIT_FAILURE;
		}
		rgbcontext = SDL_GL_CreateContext(rgbwindow);
		if (!rgbcontext)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
				"Couldn't create an OpenGL context.", NULL);
			return EXIT_FAILURE;
		}
		Assert(SDL_GL_MakeCurrent(rgbwindow, rgbcontext) == 0);
		binit = true;
	}
	loadRGBShader();
	
	return 0;
}

///////////////////////////////
#if 0
const char *  VERTEX_SHADER =
"attribute vec4 vPosition;    \n" +
"attribute vec2 a_texCoord;   \n" +
"varying vec2 tc;             \n" +
"void main()                  \n" +
"{                            \n" +
"   gl_Position = vPosition;  \n" +
"   tc = a_texCoord;          \n" +
"}                            \n";


const char *  FRAG_SHADER =
"precision mediump float;\n" +
"varying  vec2 tc;                      \n" +
"uniform sampler2D SamplerY;            \n" +
"uniform sampler2D SamplerUV;            \n" +
"const float PI = 3.14159265;           \n" +
"const mat3 convertMat = mat3( 1.0, 1.0, 1.0, 0.0, -0.39465, 2.03211, 1.13983, -0.58060, 0.0 );\n" +
"void main(void)                            \n" +
"{                                          \n" +
"vec3 yuv;                                  \n" +
"yuv.x = texture2D(SamplerY, tc).r;         \n" +
"yuv.z = texture2D(SamplerUV, tc).r - 0.5;   \n" +
"yuv.y = texture2D(SamplerUV, tc).a - 0.5;   \n" +
"vec3 color = convertMat * yuv;             \n" +
"vec4 mainColor = vec4(color, 1.0);         \n" +
"gl_FragColor =mainColor;                                       \n" +
"}   
                                                           \n";
#endif
//-------------------- -
//作者：fu_shuwu
//来源：CSDN
//原文：https ://blog.csdn.net/fu_shuwu/article/details/72983575 
//版权声明：本文为博主原创文章，转载请附上博文链接！
//////////////////////////////


#if USE_LEISHEN_YUV
int SDL_main(int argc, char* argv[])
#else
int cccSDL_main(int argc, char* argv[])
#endif
{



	// The window

	// The OpenGL context

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	// Want double-buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//ttttt();

	//With base initialization done, we can now open the window and set up the OpenGL context:
	// Create the window
	yuvwindow = SDL_CreateWindow("yuvwindow", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	printf("Version: %s\n", glGetString(GL_VERSION));
	if (!yuvwindow)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create the main window.", NULL);
		return EXIT_FAILURE;
	}
	yuvcontext = SDL_GL_CreateContext(yuvwindow);
	if (!yuvcontext)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create an OpenGL context.", NULL);
		return EXIT_FAILURE;
	}
	// Clear to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//黄色
	//glClearColor(1.0, 1.0, 0.5, 1.0);
	//GL_COLOR_BUFFER_BIT tells glClear() to only clear the screen/image (a.k.a., colour buffer).
	glClear(GL_COLOR_BUFFER_BIT);
	// Update the window，这个是SDL创建的window
	SDL_GL_SwapWindow(yuvwindow);

	//yuv
	openfile();    
	LOADSHaders();
	//ttttt();
		/*
	Normally a “real” OpenGL program would have a main loop that does things like respond to events
and render animated graphics. Since this is a really basic program, all that’s needed is to wait for the
user to click the window’s close button. SDL makes this relatively easy with its event handling
functions:
	*/
	// Wait for the user to quit
	bool quit = false;
	while (!quit) 
	{
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
#if 0 //OK
		//渲染为红色的
		glClearColor(1, 0, 0, 1);
		//black
//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
#else 
		showYUV();
//// Update the window
    SDL_GL_SwapWindow(yuvwindow);
		drawImageRGB24(pixel_w,pixel_h);
#endif
	}
	return 0;
}