#ifndef color_hpp
#define color_hpp

#include <cstdint>

namespace tga
{
	typedef uint32_t color;
	//typedef long int color;

	const color colorRShift{ 0 };
	const color colorGShift{ 8 };
	const color colorBShift{ 16 };
	const color colorAShift{ 24 };
	const color colorRMask{ 0x000000ff };
	const color colorGMask{ 0x0000ff00 };
	const color colorBMask{ 0x00ff0000 };
	const color colorRgbMask{ 0x00ffffff };
	const color colorAMask{ 0xff000000 };

	inline int getR(color c) { return (c >> colorRShift) & 0xff; }
	inline int getG(color c) { return (c >> colorGShift) & 0xff; }
	inline int getB(color c) { return (c >> colorBShift) & 0xff; }
	inline int getA(color c) { return (c >> colorAShift) & 0xff; }

	inline color rgba(const int r, const int g, const int b, const int a = 255)
	{
		return
		(
			(r << colorRShift) |
			(g << colorGShift) |
			(b << colorBShift) |
			(a << colorAShift)
		);
	}
}

#endif /* color_hpp */
