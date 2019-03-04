// ConsoleApplication2GLES.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#define  USE_GL3 1

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include  <SDL_opengles2.h>
//#include <SDL_opengl_glext.h>
#include <SDL_hints.h>
#if USE_GL3
#include <GLES3/gl3.h>
#else
#include <GLES2/gl2.h> 
#endif
#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)

int SDL_main(int argc, char* argv[])
{
	Assert(SDL_Init(SDL_INIT_VIDEO) == 0);

	SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");

	/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);*/
	//http://mickcharlesbeaver.blogspot.com/2017/11/using-opengl-es-20-with-sdl2-via-angle.html
	//When trying to use OpenGL ES 2.0 from SDL2 on MS Windows, these lines are the magic ones :
	SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
	SDL_SetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER, "none");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	
		// Want double-buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	

	SDL_Window* w = SDL_CreateWindow("ANGLE", 100, 100, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!w) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create the main window.", NULL);
				Assert(w);
		return EXIT_FAILURE;
	}

	SDL_GLContext ctx = SDL_GL_CreateContext(w);
		if (!ctx) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create an OpenGL context.", NULL);
				Assert(ctx);
		return EXIT_FAILURE;
	}


	Assert(SDL_GL_MakeCurrent(w, ctx) == 0);
	SDL_GL_SetSwapInterval(1);

	///*PFNGLGETSTRINGPROC*/ glGetString = SDL_GL_GetProcAddress("glGetString");
	///*PFNGLCLEARCOLORPROC*/ glClearColor = SDL_GL_GetProcAddress("glClearColor");
	///*PFNGLCLEARPROC*/ glClear = SDL_GL_GetProcAddress("glClear");

	//printf("GL_VERSION = %s\n", glGetString(GL_VERSION));
	//printf("GL_VENDOR = %s\n", glGetString(GL_VENDOR));
	//printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));


		//渲染为红色的
		glClearColor(1, 0, 0, 1);
        //black
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(w);

	int running = 1;
	while (running)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				running = 0;
				break;
			}
		}
		#if 0
		//渲染为红色的
		glClearColor(1, 0, 0, 1);
        //black
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(w);
        #endif
	}

	SDL_GL_DeleteContext(ctx);
	SDL_Quit();

	return 0;
}