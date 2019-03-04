// ConsoleApplication2GLES.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

//int main()
//{
//    std::cout << "Hello World!\n"; 
//}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
#include <stdio.h>
#include <SDL.h>
#include  <SDL_opengles2.h>
//#include <SDL_opengl_glext.h>
#include <SDL_hints.h>
#include <GLES2/gl2.h> 
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

	//When trying to use OpenGL ES 2.0 from SDL2 on MS Windows, these lines are the magic ones :
	SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
	SDL_SetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER, "none");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_Window* w = SDL_CreateWindow("ANGLE", 100, 100, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	Assert(w);

	SDL_GLContext ctx = SDL_GL_CreateContext(w);
	Assert(ctx);

	Assert(SDL_GL_MakeCurrent(w, ctx) == 0);
	SDL_GL_SetSwapInterval(1);

	///*PFNGLGETSTRINGPROC*/ glGetString = SDL_GL_GetProcAddress("glGetString");
	///*PFNGLCLEARCOLORPROC*/ glClearColor = SDL_GL_GetProcAddress("glClearColor");
	///*PFNGLCLEARPROC*/ glClear = SDL_GL_GetProcAddress("glClear");

	//printf("GL_VERSION = %s\n", glGetString(GL_VERSION));
	//printf("GL_VENDOR = %s\n", glGetString(GL_VENDOR));
	//printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));

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

		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(w);
	}

	SDL_GL_DeleteContext(ctx);
	SDL_Quit();

	return 0;
}