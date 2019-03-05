#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>

#include <stdint.h>

#include "eglSDL.h"
#include "initShader.h"

const uint32_t width = 800, height = 800;

GLfloat vVertices[] = {1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f};

int main(int argc, char *argv[]) {
    int run = 1;
    SDL_Window *win =
        SDL_CreateWindow("GLES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    EGL_Swap swap;
    Init_Angle_EGL(win, &swap);
    glClearColor(0.8, 0.8, 0.8, 1.0);
    GLuint program = InitShader("juliaset");
    if(!program) {
        return 0;
    }
    glBindAttribLocation(program, 0, "vPosition");
    GLuint julia_c = glGetUniformLocation(program, "julia_c");
    glUseProgram(program);
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = 0;
            } else if(event.type == SDL_MOUSEMOTION) {
                glUniform2f(julia_c, (float)event.motion.x / width * 2 - 1.5, (float)(height - event.motion.y) / height * 2 - 1);
            } else if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    run = 0;
                }
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vVertices);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        eglSwapBuffers(swap.display, swap.surface);
    }
    SDL_DestroyWindow(win);
    return 0;
}
