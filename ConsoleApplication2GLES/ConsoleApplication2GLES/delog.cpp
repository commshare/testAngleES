#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include "windows.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#define MYPRINT

void delog(const char * strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugStringA(strBuffer);
}