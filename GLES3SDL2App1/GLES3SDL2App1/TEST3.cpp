#include <SDL_egl.h>
#include <SDL_opengles2.h>
#include <SDL.h>
#include <SDL_syswm.h>

int Init_Angle_EGL(SDL_Window *win, EGL_Swap *swap)
{
	EGLint configAttribList[] =
	{
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_DEPTH_SIZE, 8,
			EGL_STENCIL_SIZE, 8,
			EGL_SAMPLE_BUFFERS, 1,
			EGL_NONE };
	EGLint surfaceAttribList[] =
	{
			EGL_POST_SUB_BUFFER_SUPPORTED_NV, EGL_FALSE,
			EGL_NONE, EGL_NONE };

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	SDL_SysWMinfo info;
	SDL_VERSION(&info.version); // initialize info structure with SDL version info
	SDL_bool get_win_info = SDL_GetWindowWMInfo(win, &info);
	SDL_assert_release(get_win_info);
	EGLNativeWindowType hWnd = info.info.win.window;

	// Get Display
	display = eglGetDisplay(GetDC(hWnd)); // EGL_DEFAULT_DISPLAY
	if (display == EGL_NO_DISPLAY)
	{
		return EGL_FALSE;
	}

	// Initialize EGL
	if (!eglInitialize(display, &majorVersion, &minorVersion))
	{
		return EGL_FALSE;
	}

	// Get configs
	if (!eglGetConfigs(display, NULL, 0, &numConfigs))
	{
		return EGL_FALSE;
	}

	// Choose config
	if (!eglChooseConfig(display, configAttribList, &config, 1, &numConfigs))
	{
		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, surfaceAttribList);
	if (surface == EGL_NO_SURFACE)
	{
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT)
	{
		return EGL_FALSE;
	}

	// Make the context current
	if (!eglMakeCurrent(display, surface, surface, context))
	{
		return EGL_FALSE;
	}

	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	swap->display = display;
	swap->surface = surface;
	return EGL_TRUE;
}

int main(int argc, char *argv[])
{

		SDL_Window *win = SDL_CreateWindow("GLES3+SDL2 Tutorial", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 600, 800,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	Init_Angle_EGL(win, &swap);
	
		while (run)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			eglSwapBuffers(swap.display, swap.surface);
		}

		return 0;
}
//-------------------- -
//作者：Zehv
//来源：CSDN
//原文：https ://blog.csdn.net/m454078356/article/details/79865749 
//版权声明：本文为博主原创文章，转载请附上博文链接！