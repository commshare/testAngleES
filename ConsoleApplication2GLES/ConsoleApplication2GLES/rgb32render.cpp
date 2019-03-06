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

/* https://github.com/BootleggersROM/frameworks_base/blob/ddd193fda654bb9099032a90999a31984604ae8f/libs/hwui/OpenGLReadback.cpp
		GLuint texture;

		GLenum format;
		GLenum internalFormat;
		GLenum type;

		switch (bitmap->colorType()) {
				case kAlpha_8_SkColorType:
						format = GL_ALPHA;
						internalFormat = GL_ALPHA;
						type = GL_UNSIGNED_BYTE;
						break;
				case kRGB_565_SkColorType:
						format = GL_RGB;
						internalFormat = GL_RGB;
						type = GL_UNSIGNED_SHORT_5_6_5;
						break;
				case kARGB_4444_SkColorType:
						format = GL_RGBA;
						internalFormat = GL_RGBA;
						type = GL_UNSIGNED_SHORT_4_4_4_4;
						break;
				case kRGBA_F16_SkColorType:
						format = GL_RGBA;
						internalFormat = GL_RGBA16F;
						type = GL_HALF_FLOAT;
						break;
				case kN32_SkColorType:
				default:
						format = GL_RGBA;
						internalFormat = GL_RGBA;
						type = GL_UNSIGNED_BYTE;
						break;
		}

				// Setup the rendertarget
		glGenTextures(1, &texture);
		caches.textureState().activateTexture(0);
		caches.textureState().bindTexture(texture);
		glPixelStorei(GL_PACK_ALIGNMENT, bitmap->bytesPerPixel());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, destWidth, destHeight, 0, format, type, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

*/

/*https://github.com/Pan-Rongtao/newbrush/blob/f72d8e68ffe7701d3bbb85a94c5489815b0d9c0b/src/gles/Texture2D.cpp

void Texture2D::loadFromData(const char *data, int width, int height, Texture::PixelFormat format)
{
	int glFormat = GL_RGBA;
	int glType = GL_UNSIGNED_BYTE;
	switch(format)
	{
	case Texture::Format_Bpp8_Alpha:		glFormat = GL_ALPHA;	glType = GL_UNSIGNED_BYTE; break;
	case Texture::Format_Bpp16_Rgb565:		glFormat = GL_RGB;		glType = GL_UNSIGNED_SHORT_5_6_5; break;
	case Texture::Format_Bpp24_Rgb888:		glFormat = GL_RGB;		glType = GL_UNSIGNED_BYTE; break;
	case Texture::Format_Bpp32_Rgb4444:		glFormat = GL_RGBA;		glType = GL_UNSIGNED_SHORT_4_4_4_4; break;
	case Texture::Format_Bpp32_Rgb5551:		glFormat = GL_RGBA;		glType = GL_UNSIGNED_SHORT_5_5_5_1; break;
	case Texture::Format_Bpp32_Rgba8888:	glFormat = GL_RGBA;		glType = GL_UNSIGNED_BYTE; break;
	}
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, glType, data);
	unbind();
	m_nPixcelWidth = width;
	m_nPixcelHeight = height;
}

*/