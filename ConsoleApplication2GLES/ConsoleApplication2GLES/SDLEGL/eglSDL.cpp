#include "pch.h"
#include "eglSDL.h"
#include <stdio.h>
#include "delog.h"
#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)
//ウィンドウプロシ�`ジャ
//Windowsでウィンドウを恬るとき駅勣
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
//兜豚晒�I尖
void initialize(EGL_Swap *swap)
{
	//ウィンドウを伏撹
	WNDCLASSEX  wc = { 0 };

	wc.cbSize = sizeof(wc);                                       //WNDCLASSEX��夛悶のサイズ
	wc.style = CS_HREDRAW | CS_VREDRAW;                             //ウインドウスタイル
	wc.lpfnWndProc = WndProc;                                       //ウィンドウプロシ�`ジャのアドレス
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);	                        //インスタンスハンドル
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);                   //アイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);                     //マウスカ�`ソル
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);       //ウインドウのクライアント�I囃の嘘尚弼
	wc.lpszMenuName = NULL;                                         //メニュ�`
	wc.lpszClassName = "window_name";                               //クラス兆
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);                 // 聞喘する弌さいアイコン

	//ウィンドウクラスを鞠�h
	RegisterClassEx(&wc);

	//ウィンドウを恬撹
	HWND hwnd = CreateWindowEx(
		0,                              //����ウィンドウスタイル
		"window_name",                  //ウィンドウクラス兆
		"window_name",                  //ウィンドウ兆
		WS_OVERLAPPEDWINDOW             //ウィンドウスタイル
		CW_USEDEFAULT,                  //ウィンドウの罪圭�鬚領志�
		CW_USEDEFAULT,                  //ウィンドウの�k圭�鬚領志�
		CW_USEDEFAULT,                  //ウィンドウの嫌
		CW_USEDEFAULT,                  //ウィンドウの互さ
		NULL,                           //�Hウィンドウハンドル
		NULL,                           //メニュ�`ハンドル
		wc.hInstance,                   //インスタンス ハンドル
		NULL                            //ウィンドウ恬撹デ�`タ
	);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	//デバイスコンテキストを函誼
	HDC hdc = GetDC(hwnd);
	Assert(hdc);
	//Windows耕嗤の�I尖は貧��まで、參和からEGLの兜豚晒

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
