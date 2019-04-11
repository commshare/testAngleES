// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#ifndef PCH_H
#define PCH_H

// TODO: 添加要在此处预编译的标头
#define EGL_EGL_PROTOTYPES 0


#define USE_EGL_ESBOOK 0
#define USE_SDL_TRIANGLE 0
#define USE_SDL2_TT_TRIAGNLE 0
#define USE_SDLEGL_ZEHV_TRIANGLE 0

#define USE_LEISHEN_YUV 1
#define USE_LEISHEN_RGB 0

#define M_PI       3.14159265358979323846   // pi
#include "delog.h"

#endif //PCH_H


/*


xiaomo@DESKTOP-E1B2EEU MINGW64 /e/mycode/GLGL/testAngleES/ConsoleApplication2GLES/ConsoleApplication2GLES (master)
$ ffmpeg -vcodec png -i 1440x900_70796_102.png -vcodec rawvideo -f rawvideo -pix_fmt rgb565 output.raw.rgb565
ffmpeg version N-92435-g759589b55e Copyright (c) 2000-2018 the FFmpeg developers
	built with gcc 8.2.1 (GCC) 20181017
	configuration: --enable-gpl --enable-version3 --enable-sdl2 --enable-fontconfig --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libfreetype --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-libshine --enable-libsnappy --enable-libsoxr --enable-libtheora --enable-libtwolame --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-libx265 --enable-libxml2 --enable-libzimg --enable-lzma --enable-zlib --enable-gmp --enable-libvidstab --enable-libvorbis --enable-libvo-amrwbenc --enable-libmysofa --enable-libspeex --enable-libxvid --enable-libaom --enable-libmfx --enable-amf --enable-ffnvcodec --enable-cuvid --enable-d3d11va --enable-nvenc --enable-nvdec --enable-dxva2 --enable-avisynth
	libavutil      56. 23.101 / 56. 23.101
	libavcodec     58. 39.100 / 58. 39.100
	libavformat    58. 22.100 / 58. 22.100
	libavdevice    58.  6.100 / 58.  6.100
	libavfilter     7. 43.100 /  7. 43.100
	libswscale      5.  4.100 /  5.  4.100
	libswresample   3.  4.100 /  3.  4.100
	libpostproc    55.  4.100 / 55.  4.100
Input #0, png_pipe, from '1440x900_70796_102.png':
	Duration: N/A, bitrate: N/A
		Stream #0:0: Video: png, rgba(pc), 1440x900 [SAR 3780:3780 DAR 8:5], 25 tbr, 25 tbn, 25 tbc
Stream mapping:
	Stream #0:0 -> #0:0 (png (native) -> rawvideo (native))
Press [q] to stop, [?] for help
[swscaler @ 0000022b389f9980] full chroma interpolation for destination format 'rgb565le' not yet implemented
Output #0, rawvideo, to 'output.raw.rgb565':
	Metadata:
		encoder         : Lavf58.22.100
		Stream #0:0: Video: rawvideo (RGB[16] / 0x10424
*/