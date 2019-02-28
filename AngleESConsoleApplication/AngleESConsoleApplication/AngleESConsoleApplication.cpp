// AngleESConsoleApplication.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>


void init(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,200.0,0.0,150.0);
}
void lineSegment(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0,0.4,0.2); //“参数”: 从“double”到“GLfloat”截断
	glBegin(GL_LINES);
	glVertex2i(180,15);
	glVertex2i(10,145);
	glEnd();
	glFlush();
}

int SimpleTestMain(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("an example opengl program ");
	init();
	glutDisplayFunc(lineSegment);
	glutMainLoop();
    std::cout << "Hello World!\n"; 
		return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

/**
 * 最简单的OpenGL播放视频的例子（OpenGL播放YUV）[Texture]
 * Simplest Video Play OpenGL (OpenGL play YUV) [Texture]
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序使用OpenGL播放YUV视频像素数据。本程序支持YUV420P的
 * 像素数据作为输入，经过转换后输出到屏幕上。其中用到了多种
 * 技术，例如Texture，Shader等，是一个相对比较复杂的例子。
 * 适合有一定OpenGL基础的初学者学习。
 *
 * 函数调用步骤如下：
 *
 * [初始化]
 * glutInit()：初始化glut库。
 * glutInitDisplayMode()：设置显示模式。
 * glutCreateWindow()：创建一个窗口。
 * glewInit()：初始化glew库。
 * glutDisplayFunc()：设置绘图函数（重绘的时候调用）。
 * glutTimerFunc()：设置定时器。
 * InitShaders()：设置Shader。包含了一系列函数，暂不列出。
 * glutMainLoop()：进入消息循环。
 *
 * [循环渲染数据]
 * glActiveTexture()：
 * glBindTexture()：
 * glTexImage2D()：
 * glUniform1i()：
 * glDrawArrays():绘制。
 * glutSwapBuffers()：显示。
 *
 * This software plays YUV raw video data using OpenGL.
 * It support read YUV420P raw file and show it on the screen.
 * It's use a slightly more complex technologies such as Texture,
 * Shaders etc. Suitable for beginner who already has some
 * knowledge about OpenGL.
 *
 * The process is shown as follows:
 *
 * [Init]
 * glutInit(): Init glut library.
 * glutInitDisplayMode(): Set display mode.
 * glutCreateWindow(): Create a window.
 * glewInit(): Init glew library.
 * glutDisplayFunc(): Set the display callback.
 * glutTimerFunc(): Set timer.
 * InitShaders(): Set Shader, Init Texture. It contains some functions about Shader.
 * glutMainLoop(): Start message loop.
 *
 * [Loop to Render data]
 * glActiveTexture(): Active a Texture
 * glBindTexture(): Bind Texture
 * glTexImage2D():
 * glUniform1i():
 * glDrawArrays(): draw.
 * glutSwapBuffers(): show.
 */

#include <stdio.h>



#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

 //Select one of the Texture mode (Set '1'):
#define TEXTURE_DEFAULT   1
//Rotate the texture
#define TEXTURE_ROTATE    0
//Show half of the Texture
#define TEXTURE_HALF      0

const int screen_w = 500, screen_h = 500;
const int pixel_w = 320, pixel_h = 180;
//YUV file
FILE *infile = NULL;
unsigned char buf[pixel_w*pixel_h * 3 / 2];
unsigned char *plane[3];


GLuint p;
GLuint id_y, id_u, id_v; // Texture id
GLuint textureUniformY, textureUniformU, textureUniformV;


#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

void display(void) {
	if (fread(buf, 1, pixel_w*pixel_h * 3 / 2, infile) != pixel_w * pixel_h * 3 / 2) {
		// Loop
		fseek(infile, 0, SEEK_SET);
		fread(buf, 1, pixel_w*pixel_h * 3 / 2, infile);
	}
	//Clear
	glClearColor(0.0, 255, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//Y
	//
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, id_y);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w, pixel_h, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);

	glUniform1i(textureUniformY, 0);
	//U
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, id_u);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w / 2, pixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
	glUniform1i(textureUniformU, 1);
	//V
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, id_v);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w / 2, pixel_h / 2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
	glUniform1i(textureUniformV, 2);

	// Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// Show
	//Double
	glutSwapBuffers();
	//Single
	//glFlush();
}

void timeFunc(int value) {
	display();
	// Timer: 40ms
	glutTimerFunc(40, timeFunc, 0);
}

char *textFileRead(char * filename)
{
	errno_t err;
	FILE * infile;
	char *s = (char *)malloc(8000);
	memset(s, 0, 8000);
	err = fopen_s(&infile,filename, "rb");
	if (err != 0)
	{
		std::cout << "open file fail " << std::endl;;
	}
	else
	{
		std::cout<<"The file "<< filename<<" was opened\n"<<std::endl;
	}
	int len = fread(s, 1, 8000, infile);
	fclose(infile);
	s[len] = 0;
	return s;
}

//Init Shader
void InitShaders()
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
	//Set Arrays
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
	//Enable it
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
	glEnableVertexAttribArray(ATTRIB_TEXTURE);


	//Init Texture
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



int main(int argc, char* argv[])
{
	//Open YUV420P file
	errno_t err;
	if ((err = fopen_s(&infile,"./test_yuv420p_320x180.yuv", "rb")!=0)) {
		printf("cannot open this file\n");
		return -1;
	}

	//YUV Data
	plane[0] = buf;
	plane[1] = plane[0] + pixel_w * pixel_h;
	plane[2] = plane[1] + pixel_w * pixel_h / 4;

	//Init GLUT
	glutInit(&argc, argv);
	//GLUT_DOUBLE
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA /*| GLUT_STENCIL | GLUT_DEPTH*/);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen_w, screen_h);
	glutCreateWindow("THANSK LEISHEN:Simplest Video Play OpenGL");
	printf("Lei Xiaohua\n");
	printf("http://blog.csdn.net/leixiaohua1020\n");
	printf("Version: %s\n", glGetString(GL_VERSION));
	GLenum l = glewInit();

	glutDisplayFunc(&display);
	glutTimerFunc(40, timeFunc, 0);

	InitShaders();

	// Begin!
	glutMainLoop();

	return 0;
}






