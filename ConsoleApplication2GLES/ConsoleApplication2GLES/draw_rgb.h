#pragma once
#include"pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <iostream>
#include "common/Include/esUtil.h"
// Basic OpenGL ES 3 + SDL2 template code
#include <SDL.h>
#include <SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>
////////////////////
#define NB_TEXTURE 1
#define G_WINDOW_FACTOR 1.0f	// 1080 / 1200 = 0.9
SDL_Window  *rgbwindow;
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
#define RGB24_yangying "yangying640_400.raw.rgb24"
SDL_GLContext rgbcontext;
#define Assert(x) do {if (!(x)) __debugbreak(); } while (0)
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
//rgb file
FILE *fprgb = NULL;
const int bpp = 24;
const int pixel_w = 640, pixel_h = 400;
unsigned char buffer[pixel_w*pixel_h*bpp / 8];
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
void drawImageRGB24(int width , int height) {
	static bool first = true;
	if (first)
	{
		fprgb = fopen(/*"./test_rgb24_320x180.rgb"*//*"./288_180_output.raw.rgb565.rgb"*/RGB24_yangying, "rb+");
		first = false;
	}
	if (fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fprgb) != pixel_w * pixel_h*bpp / 8)
	{
		// Loop
		fseek(fprgb, 0, SEEK_SET);
		fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fprgb);
	}
	unsigned char * data = buffer;
	Assert(SDL_GL_MakeCurrent(rgbwindow, rgbcontext) == 0);
	// Set the viewport 
	glViewport(0, 100, 400, 400);

	//glMatrixMode(GL_PROJECTION);
	// Use the program object
	//shader.use();
	glUseProgram(programID);

	for (int i = 0; i < NB_TEXTURE; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		//　glBindTexture函数实现了将调用glGenTextures函数生成的纹理的名字绑定到对应的目标纹理上
		glBindTexture(GL_TEXTURE_2D, texture[i]);
	}
	//glTexImage2D在OpenGL中主要用来指定二维纹理和立方体纹理
	/*
	1.当有缓冲区绑定到 GL_PIXEL_PACK/UNPACK_BUFFER 时，这时候使用了PBO(Pixel Buffer Object)，此时的data是一个指向缓冲区对象数据的偏移量
2.当没有缓冲区绑定到GL_PIXEL_PACK/UNPACK_BUFFER 时，这是data是指向内存中的指针(上文中提到的pBytes)
原文：https://blog.csdn.net/csxiaoshui/article/details/27543615 
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);	// grey background
	glClear(GL_COLOR_BUFFER_BIT);			// clear color buffer
	glDrawArrays(GL_TRIANGLES, 0, 6);
	SDL_GL_SwapWindow(rgbwindow);
}