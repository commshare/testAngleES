#ifndef EGL_SDL_H
#define EGL_SDL_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <SDL.h>
#include <SDL_egl.h>
#include <SDL_opengles2.h>
#include <SDL_syswm.h>

typedef struct {
    EGLDisplay display;
    EGLSurface surface;
} EGL_Swap;

int Init_Angle_EGL(SDL_Window *win, EGL_Swap *swap);

#define setTitle() do {                                             \
    char title[128];                                                \
    sprintf(title, "%s %s", glGetString(GL_VERSION), __FILE__);     \
    SDL_SetWindowTitle(win, title);                                 \
} while(0)

//#ifdef __cplusplus
//}
//#endif

#endif