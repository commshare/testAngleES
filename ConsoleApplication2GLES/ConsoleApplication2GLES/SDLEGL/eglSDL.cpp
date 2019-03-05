#include "pch.h"
#include "eglSDL.h"
#include <stdio.h>
#include "delog.h"
#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)
//������ɥ��ץ��`����
//Windows�ǥ�����ɥ�������Ȥ���Ҫ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
//http://bkmts.xsrv.jp/opengl-es2/
#if 0
//���ڻ��I��
void initialize(EGL_Swap *swap)
{
	//������ɥ�������
	WNDCLASSEX  wc = { 0 };

	wc.cbSize = sizeof(wc);                                       //WNDCLASSEX������Υ�����
	wc.style = CS_HREDRAW | CS_VREDRAW;                             //������ɥ���������
	wc.lpfnWndProc = WndProc;                                       //������ɥ��ץ��`����Υ��ɥ쥹
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);	                        //���󥹥��󥹥ϥ�ɥ�
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);                   //��������
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);                     //�ޥ������`����
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);       //������ɥ��Υ��饤������I��α���ɫ
	wc.lpszMenuName = NULL;                                         //��˥�`
	wc.lpszClassName = "window_name";                               //���饹��
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);                 // ʹ�ä���С������������

	//������ɥ����饹����h
	RegisterClassEx(&wc);

	//������ɥ�������
	HWND hwnd = CreateWindowEx(
		0,                              //����������ɥ���������
		"window_name",                  //������ɥ����饹��
		"window_name",                  //������ɥ���
		WS_OVERLAPPEDWINDOW             //������ɥ���������
		CW_USEDEFAULT,                  //������ɥ��κ᷽���λ��
		CW_USEDEFAULT,                  //������ɥ��οk�����λ��
		CW_USEDEFAULT,                  //������ɥ��η�
		CW_USEDEFAULT,                  //������ɥ��θߤ�
		NULL,                           //�H������ɥ��ϥ�ɥ�
		NULL,                           //��˥�`�ϥ�ɥ�
		wc.hInstance,                   //���󥹥��� �ϥ�ɥ�
		NULL                            //������ɥ����ɥǩ`��
	);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	//�ǥХ�������ƥ����Ȥ�ȡ��
	HDC hdc = GetDC(hwnd);
	Assert(hdc);
	//Windows���Ф΄I�����ӛ�ޤǡ����¤���EGL�γ��ڻ�

	EGLDisplay display = eglGetDisplay(hdc);
	Assert(display);

	EGLint major, minor;
	Assert(eglInitialize(display, &major, &minor));

	Assert(eglBindAPI(EGL_OPENGL_ES_API));

	//config
	const EGLint configAttributes[] =
	{
		EGL_RENDERABLE_TYPE,
		EGL_OPENGL_ES3_BIT,
		EGL_BUFFER_SIZE,
		32,
		EGL_NONE
	};
	EGLConfig config;
	EGLint num_config;

	Assert(eglChooseConfig(display, configAttributes, &config, 1, &num_config));

	//context
	const EGLint contextAttributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION,
		3,
		EGL_NONE
	};

	EGLContext context = eglCreateContext(display, config, NULL, contextAttributes);
	Assert(context);

	//surface
	EGLSurface surface = eglCreateWindowSurface(display, config, hwnd, NULL);
	Assert(surface);
	Assert(eglMakeCurrent(display, surface, surface, context));

	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_SHADING_LANGUAGE_VERSION: %s\n",
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	swap->display = display;
	swap->surface = surface;
}

#endif

int Init_Angle_EGL(SDL_Window *win, EGL_Swap *swap) {

    EGLint configAttribList[] = {
        EGL_RED_SIZE,       8, EGL_GREEN_SIZE,   8,
        EGL_BLUE_SIZE,      8, EGL_ALPHA_SIZE,   8 /*: EGL_DONT_CARE*/,
        EGL_DEPTH_SIZE,     8, EGL_STENCIL_SIZE, 8,
        EGL_SAMPLE_BUFFERS, 1, EGL_NONE};
    EGLint surfaceAttribList[] = {EGL_POST_SUB_BUFFER_SUPPORTED_NV, EGL_FALSE,
                                  EGL_NONE, EGL_NONE};

    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE,
                               EGL_NONE};

    SDL_SysWMinfo info;
    SDL_VERSION(
        &info.version); // initialize info structure with SDL version info
    SDL_bool get_win_info = SDL_GetWindowWMInfo(win, &info);
    SDL_assert_release(get_win_info);
    EGLNativeWindowType hWnd = info.info.win.window;

		HDC hdc = GetDC(hWnd);
		Assert(hdc);

    // Get Display
    display = eglGetDisplay(hdc); // EGL_DEFAULT_DISPLAY //GetDC(hWnd)
		Assert(display);
    if (display == EGL_NO_DISPLAY)
		{
			  delog("eglGetDisplay fail !!!! \n");
        return EGL_FALSE;
    }

    // Initialize EGL
    if (!eglInitialize(display, &majorVersion, &minorVersion)) {
        return EGL_FALSE;
    }

    // Get configs
    if (!eglGetConfigs(display, NULL, 0, &numConfigs)) {
        return EGL_FALSE;
    }

    // Choose config
    if (!eglChooseConfig(display, configAttribList, &config, 1, &numConfigs)) {
        return EGL_FALSE;
    }

    // Create a surface
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd,
                                     surfaceAttribList);
    if (surface == EGL_NO_SURFACE) {
        return EGL_FALSE;
    }

    // Create a GL context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT) {
        return EGL_FALSE;
    }

    // Make the context current
    if (!eglMakeCurrent(display, surface, surface, context)) {
        return EGL_FALSE;
    }

    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n",
           glGetString(GL_SHADING_LANGUAGE_VERSION));
    swap->display = display;
    swap->surface = surface;
    return EGL_TRUE;
}
