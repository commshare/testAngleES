#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL2/SDL_opengles2.h>

#include <stdint.h>

#include "eglSDL.h"
#include "initShader.h"

#define width 800
#define height 800
#define fbo_w (width / 2)
#define fbo_h (height / 2)

GLfloat vVertices[] = {1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f};

typedef struct {
    GLuint program;
    GLuint vPosition;
    GLuint julia_c;
} gl_julia;

gl_julia julia;

typedef struct {
    GLuint program;
    GLuint texture;
    GLuint a_Position;
    GLuint u_MVP;
    GLuint u_Sampler;
} gl_plane;

gl_plane plane;

typedef struct {
    GLuint fbo;
    GLuint tex;
} gl_fbo;

gl_fbo fbo;

GLuint InitJulia() {
    GLuint program = InitShader("juliaset");
    if (program == 0) {
        return 0;
    }
    julia.program = program;
    julia.vPosition = glGetAttribLocation(program, "vPosition");
    julia.julia_c = glGetUniformLocation(program, "julia_c");
    return program;
}

GLuint InitPlane() {
    GLuint program = InitShader("fbo_test");
    if (program == 0) {
        return 0;
    }
    plane.program = program;
    plane.a_Position = glGetAttribLocation(program, "a_Position");
    plane.u_Sampler = glGetUniformLocation(program, "u_Sampler");

    return program;
}

int InitFrameBuffer() {
    glGenTextures(1, &fbo.tex);
    if (!fbo.tex) {
        printf("err tex\n");
    }

    glBindTexture(GL_TEXTURE_2D, fbo.tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_w, fbo_h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &fbo.fbo);
    if (!fbo.fbo) {
        printf("err fbo\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           fbo.tex, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return fbo.fbo;
}

void DrawJulia(float a, float b) {
    glUseProgram(julia.program);
    glVertexAttribPointer(julia.vPosition, 2, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(julia.vPosition);
    glUniform2f(julia.julia_c, a, b);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void DrawPlane() {
    glUseProgram(plane.program);
    glVertexAttribPointer(plane.a_Position, 2, GL_FLOAT, GL_FALSE, 0,
                          vVertices);
    glEnableVertexAttribArray(plane.a_Position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo.tex);
    glUniform1i(plane.u_Sampler, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(int argc, char *argv[]) {
    int run = 1;
    float a = 0, b = 0;
    SDL_Window *win =
        SDL_CreateWindow("GLES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Surface *sur = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32,
                                                      SDL_PIXELFORMAT_RGBA32);
    EGL_Swap swap;
    Init_Angle_EGL(win, &swap);
    setTitle();
    InitJulia();
    if (!InitPlane()) {
        return 0;
    }
    glClearColor(0.8, 0.8, 0.8, 1.0);

    InitFrameBuffer();
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = 0;
            } else if (event.type == SDL_MOUSEMOTION) {
                a = (float)event.motion.x / width * 2 - 1.5;
                b = (float)(height - event.motion.y) / height * 2 - 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_c:
                    memset(sur->pixels, 0xff, width * height * 3);
                    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                                 sur->pixels);
                    SDL_SaveBMP(sur, "save.bmp");
                    break;
                case SDLK_ESCAPE:
                    run = 0;
                    break;
                }
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
        glViewport(0, 0, fbo_w, fbo_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawJulia(a, b);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawPlane();
        eglSwapBuffers(swap.display, swap.surface);
    }
    SDL_FreeSurface(sur);
    SDL_DestroyWindow(win);
    return 0;
}
