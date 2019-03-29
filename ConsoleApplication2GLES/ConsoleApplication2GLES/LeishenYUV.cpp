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
#include "draw_rgb.h"
SDL_GLContext yuvcontext = NULL;
SDL_Window *yuvwindow = NULL;
//Select one of the Texture mode (Set '1'):
#define TEXTURE_DEFAULT   1
//Rotate the texture
#define TEXTURE_ROTATE    0
//Show half of the Texture
#define TEXTURE_HALF      0
#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)

const int screen_w = 500, screen_h = 500;
const int ypixel_w = 320, ypixel_h = 180;
//YUV file
FILE *infile = NULL;
unsigned char buf[ypixel_w*ypixel_h * 3 / 2];
unsigned char *plane[3];

GLuint p;
GLuint id_y, id_u, id_v; // Texture id
GLuint textureUniformY, textureUniformU, textureUniformV;


#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

void showYUV()
{
	//从infile读取一帧yuv到buf里
	if (fread(buf, 1, ypixel_w*ypixel_h * 3 / 2, infile) != ypixel_w * ypixel_h * 3 / 2)
	{
		// Loop
		fseek(infile, 0, SEEK_SET);
		fread(buf, 1, ypixel_w*ypixel_h * 3 / 2, infile);
		fread(buf, 1, ypixel_w*ypixel_h * 3 / 2, infile);
	}
	Assert(SDL_GL_MakeCurrent(yuvwindow, yuvcontext) == 0);

	//Clear
	glClearColor(0.0, 255, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Y
  //
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id_y);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ypixel_w, ypixel_h, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);
	glUniform1i(textureUniformY, 0);

	//U
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, id_u);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ypixel_w / 2, ypixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
	glUniform1i(textureUniformU, 1);

	//V
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, id_v);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ypixel_w / 2, ypixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
	glUniform1i(textureUniformV, 2);

	// Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	SDL_GL_SwapWindow(yuvwindow);
}

char *textFileRead(char * filename)
{
	errno_t err;
	FILE * infile;
	char *s = (char *)malloc(8000);
	memset(s, 0, 8000);
	err = fopen_s(&infile, filename, "rb");
	if (err != 0)
	{
		std::cout << "open file fail " << std::endl;;
	}
	else
	{
		std::cout << "The file " << filename << " was opened\n" << std::endl;
	}
	int len = fread(s, 1, 8000, infile);
	fclose(infile);
	s[len] = 0;
	return s;
}

//Init Shader
void LOADSHaders()
{
	GLint vertCompiled, fragCompiled, linked;

	GLint v, f;
	const char *vs, *fs;
	//Shader: step1
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	//Get source code
	char  vsh[] = "Shader.vsh";
	char  fsh[] = "Shader.fsh";
	vs = textFileRead(vsh);
	fs = textFileRead(fsh);
	//Shader: step2
	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	//Shader: step3
	glCompileShader(v);
	//Debug
	glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);

	//Program: Step1
	p = glCreateProgram();
	//Program: Step2
	glAttachShader(p, v);
	glAttachShader(p, f);

	glBindAttribLocation(p, ATTRIB_VERTEX, "vertexIn");
	glBindAttribLocation(p, ATTRIB_TEXTURE, "textureIn");
	//Program: Step3
	glLinkProgram(p);
	//Debug
	glGetProgramiv(p, GL_LINK_STATUS, &linked);
	//Program: Step4
	glUseProgram(p);


	//Get Uniform Variables Location
	textureUniformY = glGetUniformLocation(p, "tex_y");
	textureUniformU = glGetUniformLocation(p, "tex_u");
	textureUniformV = glGetUniformLocation(p, "tex_v");

#if TEXTURE_ROTATE
	static const GLfloat vertexVertices[] = {
			-1.0f, -0.5f,
			 0.5f, -1.0f,
			-0.5f,  1.0f,
			 1.0f,  0.5f,
	};
#else
	static const GLfloat vertexVertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
	};
#endif

#if TEXTURE_HALF
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		0.5f,  1.0f,
		0.0f,  0.0f,
		0.5f,  0.0f,
	};
#else
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,
	};
#endif

	//顶点坐标的数组
	//Set Arrays
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
	//Enable it
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
	glEnableVertexAttribArray(ATTRIB_TEXTURE);


	//YUV文理句柄
	glGenTextures(1, &id_y);
	glBindTexture(GL_TEXTURE_2D, id_y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &id_u);
	glBindTexture(GL_TEXTURE_2D, id_u);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &id_v);
	glBindTexture(GL_TEXTURE_2D, id_v);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

int openfile()
{
	//Open YUV420P file
	errno_t err;
	if ((err = fopen_s(&infile, "./test_yuv420p_320x180.yuv", "rb") != 0)) {
		printf("cannot open this file\n");
		return -1;
	}
	//全局变量plane
	//YUV Data
	plane[0] = buf;
	plane[1] = plane[0] + ypixel_w * ypixel_h;
	plane[2] = plane[1] + ypixel_w * ypixel_h / 4;
	return 0;
}

int ttttt()
{
	static bool binit = false;

	SDL_GLContext context;
	if (!binit)
	{
		//With base initialization done, we can now open the window and set up the OpenGL context:
// Create the window
		rgbwindow = SDL_CreateWindow("rgbwindow", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (!rgbwindow)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
				"Couldn't create the main window.", NULL);
			return EXIT_FAILURE;
		}
		rgbcontext = SDL_GL_CreateContext(rgbwindow);
		if (!rgbcontext)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
				"Couldn't create an OpenGL context.", NULL);
			return EXIT_FAILURE;
		}
		Assert(SDL_GL_MakeCurrent(rgbwindow, rgbcontext) == 0);
		binit = true;
	}
	loadRGBShader();
	
	return 0;
}

#if USE_LEISHEN_YUV
int SDL_main(int argc, char* argv[])
#else
int cccSDL_main(int argc, char* argv[])
#endif
{



	// The window

	// The OpenGL context

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
	yuvwindow = SDL_CreateWindow("yuvwindow", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	printf("Version: %s\n", glGetString(GL_VERSION));
	if (!yuvwindow)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create the main window.", NULL);
		return EXIT_FAILURE;
	}
	yuvcontext = SDL_GL_CreateContext(yuvwindow);
	if (!yuvcontext)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
			"Couldn't create an OpenGL context.", NULL);
		return EXIT_FAILURE;
	}
	// Clear to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//GL_COLOR_BUFFER_BIT tells glClear() to only clear the screen/image (a.k.a., colour buffer).
	glClear(GL_COLOR_BUFFER_BIT);
	// Update the window，这个是SDL创建的window
	SDL_GL_SwapWindow(yuvwindow);


	openfile();
	LOADSHaders();
	ttttt();
		/*
	Normally a “real” OpenGL program would have a main loop that does things like respond to events
and render animated graphics. Since this is a really basic program, all that’s needed is to wait for the
user to click the window’s close button. SDL makes this relatively easy with its event handling
functions:
	*/
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
#else 
		showYUV();
//// Update the window
    SDL_GL_SwapWindow(yuvwindow);
		drawImageRGB24(pixel_w,pixel_h);
#endif
	}
	return 0;
}