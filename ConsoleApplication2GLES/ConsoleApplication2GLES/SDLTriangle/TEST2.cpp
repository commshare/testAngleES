//https://handmade.network/forums/t/2055-sdl2_angle_porting
#include <stdio.h>
#include <SDL.h>
#include <GLES2/gl2.h> // https://www.khronos.org/registry/OpenGL/api/GLES2/gl2.h

#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)

int fffmain(int argc, char* argv[])
{
	Assert(SDL_Init(SDL_INIT_VIDEO) == 0);

//	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");
	SDL_SetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER, "1");

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	SDL_Window* w = SDL_CreateWindow("ANGLE", 100, 100, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	Assert(w);

	SDL_GLContext ctx = SDL_GL_CreateContext(w);
	Assert(ctx);

	Assert(SDL_GL_MakeCurrent(w, ctx) == 0);
	SDL_GL_SetSwapInterval(1);

	PFNGLGETSTRINGPROC glGetString = (PFNGLGETSTRINGPROC)SDL_GL_GetProcAddress("glGetString");
	PFNGLCLEARCOLORPROC glClearColor = (PFNGLCLEARCOLORPROC)SDL_GL_GetProcAddress("glClearColor");
	PFNGLCLEARPROC glClear = (PFNGLCLEARPROC)SDL_GL_GetProcAddress("glClear");

	printf("GL_VERSION = %s\n", glGetString(GL_VERSION));
	printf("GL_VENDOR = %s\n", glGetString(GL_VENDOR));
	printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));

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