#include "pch.h"
#include <SDL.h>
//https://blog.csdn.net/m454078356/article/details/79865749
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#include <stdint.h>

#include "eglSDL.h"
#include "initShader.h"

const uint32_t width = 640, height = 480;

GLfloat vVertices[] = {0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};

GLfloat vColors[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};

#if USE_SDLEGL_ZEHV_TRIANGLE
int SDL_main(int argc, char *argv[]) 
#else
int aaaafSDL_main(int argc, char *argv[])
#endif
{
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

    int run = 1;
    SDL_Window *win =
        SDL_CreateWindow("GLES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    EGL_Swap swap;
    Init_Angle_EGL(win, &swap);
    glClearColor(0.8, 0.8, 0.8, 1.0);
    GLuint program = ZInitShader("triangle");
    if(!program) {

        return 0;
    }
    glBindAttribLocation(program, 0, "vPosition");
    glBindAttribLocation(program, 1, "vColor");
    GLuint vMouse = glGetUniformLocation(program, "vMouse");
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = 0;
            } else if(event.type == SDL_MOUSEMOTION) {
                glUniform2f(vMouse, event.motion.x, height - event.motion.y);
            }
        }
        glUseProgram(program);
        glClear(GL_COLOR_BUFFER_BIT);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vColors);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        eglSwapBuffers(swap.display, swap.surface);
    }

    SDL_DestroyWindow(win);
    return 0;
}