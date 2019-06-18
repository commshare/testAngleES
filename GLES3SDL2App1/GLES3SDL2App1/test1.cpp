// Basic OpenGL ES 3 + SDL2 template code
#include <SDL.h>
#include <SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>
const unsigned int DISP_WIDTH = 640;
const unsigned int DISP_HEIGHT = 480;
int SDL_main(int argc, char *args[]) {
	// The window
	SDL_Window *window = NULL;
	// The OpenGL context
	SDL_GLContext context = NULL;
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 10;
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
	glClear(GL_COLOR_BUFFER_BIT);
	// Update the window
	SDL_GL_SwapWindow(window);
	// Wait for the user to quit
	bool quit = false;
	while (!quit) {
			SDL_Event event;
		if (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				// User wants to quit
				quit = true;
			}
		}
	}
	return EXIT_SUCCESS;
}