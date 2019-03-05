#include "pch.h"
#include "initShader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLuint ZLoadShader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;
    // Create the shader object
    shader = glCreateShader(type);
    if (shader == 0)
        return 0;
    // Load the shader source
    glShaderSource(shader, 1, (const GLchar *const *)&shaderSrc, NULL);
    // Compile the shader
    glCompileShader(shader);
    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            printf("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int ZLoadShaderFile(GLenum type, const char *file) {
    FILE *shaderFile;
    char *buffer;
    shaderFile = fopen(file, "r");
    if (!shaderFile) {
        return 0;
    }
    fseek(shaderFile, 0, SEEK_END);
    size_t fileSize = ftell(shaderFile);
    rewind(shaderFile);
    buffer = (char *)malloc(fileSize);
    memset(buffer, 0, fileSize);
    fread(buffer, 1, fileSize, shaderFile);
    fclose(shaderFile);
    GLuint shader = ZLoadShader(type, buffer);
    free(buffer);
    return shader;
}

GLuint LoadShaderProgram(const char *shaderName) {
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    char fileName[128];
    sprintf(fileName, "shader/%s.vert", shaderName);
    vertexShader = ZLoadShaderFile(GL_VERTEX_SHADER, fileName);
    sprintf(fileName, "shader/%s.frag", shaderName);
    fragmentShader = ZLoadShaderFile(GL_FRAGMENT_SHADER, fileName);
    programObject = glCreateProgram();
    if (programObject == 0)
        return 0;
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    return programObject;
}

GLuint ZInitShader(const char *shaderSrc) {
    GLuint program = LoadShaderProgram(shaderSrc);
    if (program == 0) 
		{

        return 0;
    }
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) 
		{
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);

            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            printf("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}