// AngleESConsoleApplication.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
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
	glColor3f(0.0,0.4,0.2);
	glBegin(GL_LINES);
	glVertex2i(180,15);
	glVertex2i(10,145);
	glEnd();
	glFlush();
}

int main(int argc,char **argv)
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
