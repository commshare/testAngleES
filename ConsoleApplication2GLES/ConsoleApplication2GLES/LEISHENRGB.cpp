#include"pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <iostream>
// Basic OpenGL ES 3 + SDL2 template code
#include <SDL.h>
#include <SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>

#include "common/Include/esUtil.h"
#include "shaderSrc.h"
#define NB_TEXTURE 1
#define G_WINDOW_FACTOR 1.0f	// 1080 / 1200 = 0.9
#define TEX_FORMAT 	GL_RGB
#define TEX_TYPE 	GL_UNSIGNED_SHORT_5_6_5
#define TEX_CV_CVT 	cv::COLOR_BGR2BGR565

#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)

////////////////////
GLuint programID;
// OpenGL objects indices
GLuint VBO;			// Vertex Buffer Object
GLuint texture[NB_TEXTURE];
// Attributes locations:
GLint a_position;
GLint a_texcoord;
// Uniforms locations:
GLint texSelect;
GLint u_width;
GLint u_coef;

const GLfloat vertices[30] = {
	// positions:(x,y,z)    // texture coords:(s,t)
	-1.0f, -1.0f * G_WINDOW_FACTOR, 0.0f,		0.0f, 1.0f,   // bottom left
	1.0f, -1.0f * G_WINDOW_FACTOR, 0.0f,		1.0f, 1.0f,   // bottom right
	1.0f,  1.0f * G_WINDOW_FACTOR, 0.0f,		1.0f, 0.0f,   // top right
	-1.0f, -1.0f * G_WINDOW_FACTOR, 0.0f,		0.0f, 1.0f,   // bottom left
	1.0f,  1.0f * G_WINDOW_FACTOR, 0.0f,		1.0f, 0.0f,   // top right
	-1.0f,  1.0f * G_WINDOW_FACTOR, 0.0f,		0.0f, 0.0f    // top left
};
/////////////////////////////
const char* vertShaderSrc = "precision mediump float;\n"
"attribute vec3 a_position;\n"
"attribute vec2 a_texcoord;\n"
"varying vec2 v_texcoord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(a_position, 1.0);\n"
"    v_texcoord= a_texcoord;\n"
"}";
const char* fragShaderSrc = "precision mediump float;\n"
"varying vec2 v_texcoord;\n"
"uniform sampler2D texture0;\n"
"void main()\n"
"{\n"
"	gl_FragColor = texture2D(texture0, v_texcoord);\n"
"}";
/////////////////////



#define yangying "yangying640_400.raw.rgb565"
#define rgb321080p "Image1920x1080.rgb"
#define rgba640400yangying  "yangying640_400.raw.rgba"
#define RGB24_yangying "yangying640_400.raw.rgb24"
//set '1' to choose a type of file to play
#define LOAD_RGB24   1
#define LOAD_BGR24   0
#define LOAD_BGRA    0
#define LOAD_YUV420P 0
#define LOAD_RGB32 0

int screen_w = 800, screen_h = 600;
const int pixel_w = 640, pixel_h = 400;
const int pixel_rgb32_w = 1920, pixel_rgb32_h = 1080;
//Bit per Pixel
#if LOAD_BGRA
const int bpp = 32;
#elif LOAD_RGB24|LOAD_BGR24
const int bpp = 24;
#elif LOAD_YUV420P
const int bpp = 12;
#elif LOAD_RGB32
const int bpp = 32;
#endif
//YUV file
FILE *fp = NULL;
unsigned char buffer[pixel_w*pixel_h*bpp / 8];
unsigned char buffer_convert[pixel_w*pixel_h * 3];

inline unsigned char CONVERT_ADJUST(double tmp)
{
	return (unsigned char)((tmp >= 0 && tmp <= 255) ? tmp : (tmp < 0 ? 0 : 255));
}


void drawImageBGR565(int width, int height, unsigned char * data) {
	// Set the viewport  //图形最终显示到屏幕的区域的位置、长和宽
	glViewport(0, 0, screen_w, screen_h);

	//指定矩阵
	//glMatrixMode(GL_PROJECTION);
	// Use the program object
	//shader.use();
	glUseProgram(programID);

	for (int i = 0; i < NB_TEXTURE; i++) 
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	// grey background
	glClear(GL_COLOR_BUFFER_BIT);			// clear color buffer
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void drawImageRGBA(int width, int height, unsigned char * data) {
	// Set the viewport  //图形最终显示到屏幕的区域的位置、长和宽
	glViewport(0, 0, 400, 400);

	//指定矩阵
	//glMatrixMode(GL_PROJECTION);
	// Use the program object
	//shader.use();
	glUseProgram(programID);

	for (int i = 0; i < NB_TEXTURE; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
	}
	//这里改了
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	// grey background
	glClear(GL_COLOR_BUFFER_BIT);			// clear color buffer
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void drawImageRGB32(int width, int height, unsigned char * data) {
	// Set the viewport  //图形最终显示到屏幕的区域的位置、长和宽
	glViewport(0, 0, 400, 400);

	//指定矩阵
	//glMatrixMode(GL_PROJECTION);
	// Use the program object
	//shader.use();
	glUseProgram(programID);

	for (int i = 0; i < NB_TEXTURE; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
	}
	//这里改了
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	// grey background
	glClear(GL_COLOR_BUFFER_BIT);			// clear color buffer
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void drawImageRGB24(int width, int height, unsigned char * data) {
	// Set the viewport  //图形最终显示到屏幕的区域的位置、长和宽
	glViewport(0, 0, 400, 400);

	//指定矩阵
	//glMatrixMode(GL_PROJECTION);
	// Use the program object
	//shader.use();
	glUseProgram(programID);

	for (int i = 0; i < NB_TEXTURE; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
	}
	//这里改了
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	// grey background
	glClear(GL_COLOR_BUFFER_BIT);			// clear color buffer
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
int showRGB()
{
	
		if (fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fp) != pixel_w * pixel_h*bpp / 8) 
		{
			// Loop
			fseek(fp, 0, SEEK_SET);
			fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fp);
		}
#if 0
		//Make picture full of window
		//Move to(-1.0,1.0)
		glRasterPos3f(-1.0f, 1.0f, 0);
		//Zoom, Flip
		glPixelZoom((float)screen_w / (float)pixel_w, -(float)screen_h / pixel_h);
#endif
		#if LOAD_BGRA
			drawImageRGBA(pixel_w, pixel_h, buffer);
		#elif LOAD_RGB32
			drawImageRGB32(pixel_w, pixel_h, buffer);
		#elif LOAD_RGB24
		  drawImageRGB24(pixel_w, pixel_h, buffer);
		#else
			drawImageBGR565(pixel_w, pixel_h, buffer);
		#endif
//#if LOAD_BGRA
//		glDrawPixels(pixel_w, pixel_h, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
//#elif LOAD_RGB24
//		glDrawPixels(pixel_w, pixel_h, GL_RGB, GL_UNSIGNED_BYTE, buffer);
//#elif LOAD_BGR24
//		glDrawPixels(pixel_w, pixel_h, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
//#elif LOAD_YUV420P
//		CONVERT_YUV420PtoRGB24(buffer, buffer_convert, pixel_w, pixel_h);
//		glDrawPixels(pixel_w, pixel_h, GL_RGB, GL_UNSIGNED_BYTE, buffer_convert);
//#endif
//		//GLUT_DOUBLE
//		glutSwapBuffers();

		//GLUT_SINGLE
		//glFlush();
		return 0;
}
void loadRGBShader()
{
	programID = esLoadProgram(vertShaderSrc, fragShaderSrc);

	// Get location of the attributes after the shader is linked
	a_position = glGetAttribLocation(programID, "a_position");
	a_texcoord = glGetAttribLocation(programID, "a_texcoord");
	if (NB_TEXTURE > 1)
		texSelect = glGetUniformLocation(programID, "u_texSelect");

	// set-up vertex data, buffers and attributes
// ------------------------------------------
// Generate, bind and set data of the Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	const GLuint POS_SIZE = 3;		// position is 3 dimensions
	const GLuint TEXCOORD_SIZE = 2;	// texcoord is 2 dimensions
	const GLuint STRIDE = (POS_SIZE + TEXCOORD_SIZE) * sizeof(GLfloat);	// stride in bytes between each attribute
	size_t offset = 0;

	// position attribute
	glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, STRIDE, (void*)offset);
	glEnableVertexAttribArray(a_position);
	offset += POS_SIZE * sizeof(float);
	// texture attribute
	glVertexAttribPointer(a_texcoord, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)offset);
	glEnableVertexAttribArray(a_texcoord);
	offset += TEXCOORD_SIZE * sizeof(float);

	// texture configuration
	for (int i = 0; i < NB_TEXTURE; i++)
	{
		glGenTextures(1, &texture[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		// texture wrapping (repeat) and filtering (linear)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

}
#if USE_LEISHEN_RGB
int SDL_main(int argc, char* argv[])
#else
int fffeeeSDL_main(int argc, char* argv[])
#endif
{
#if LOAD_BGRA
	fp = fopen(/*"../test_bgra_320x180.rgb"*/rgba640400yangying, "rb+");
#elif LOAD_RGB24
	fp = fopen(/*"./test_rgb24_320x180.rgb"*//*"./288_180_output.raw.rgb565.rgb"*/RGB24_yangying, "rb+");
#elif LOAD_RGB565
	fp = fopen(/*"./test_rgb24_320x180.rgb"*//*"./288_180_output.raw.rgb565.rgb"*/yangying, "rb+");
#elif LOAD_BGR24
	fp = fopen("../test_bgr24_320x180.rgb", "rb+");
#elif LOAD_YUV420P
	fp = fopen("../test_yuv420p_320x180.yuv", "rb+");
#elif LOAD_RGB32
	fp = fopen(rgb321080p, "rb+");
#endif
	if (fp == NULL) 
	{
		printf("Cannot open this file.\n");
		return -1;
	}

	// The window
	SDL_Window *window = NULL;
	// The OpenGL context
	SDL_GLContext context = NULL;
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	// Setup the exit hook
	atexit(SDL_Quit);
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



	//With base initialization done, we can now open the window and set up the OpenGL context:
	// Create the window
	window = SDL_CreateWindow("GLES3+SDL2 Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create the main window.", NULL);
		return EXIT_FAILURE;
	}
	context = SDL_GL_CreateContext(window);
	if (!context)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create an OpenGL context.", NULL);
		return EXIT_FAILURE;
	}
	Assert(SDL_GL_MakeCurrent(window, context) == 0);
	SDL_GL_SetSwapInterval(1);

	// Clear to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//GL_COLOR_BUFFER_BIT tells glClear() to only clear the screen/image (a.k.a., colour buffer).
	glClear(GL_COLOR_BUFFER_BIT);
	// Update the window，这个是SDL创建的window
	SDL_GL_SwapWindow(window);
	
	loadRGBShader();

	// Wait for the user to quit
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		/*
		SDL_WaitEvent() stops the program until an event comes in. If the incoming event is an SDL_QUIT,
then the code above exits the while loop, and quits.
		*/
		if (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT) {
				// User wants to quit
				quit = true;
				break;
			}
		}
#if 0 //OK
		//渲染为红色的
		glClearColor(1, 0, 0, 1);
		//black
//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
#else //三角形
		//// Now draw!

		showRGB();

		//// Update the window
		SDL_GL_SwapWindow(window);
#endif
	}
}