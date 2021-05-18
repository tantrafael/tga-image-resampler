#ifndef color_hpp
#define color_hpp

namespace tga
{
	typedef uint32_t color;

	const color color_r_shift  = 0;
	const color color_g_shift  = 8;
	const color color_b_shift  = 16;
	const color color_a_shift  = 24;
	const color color_r_mask   = 0x000000ff;
	const color color_g_mask   = 0x0000ff00;
	const color color_b_mask   = 0x00ff0000;
	const color color_rgb_mask = 0x00ffffff;
	const color color_a_mask   = 0xff000000;

	inline color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
	{
		return
		(
			(r << color_r_shift) |
			(g << color_g_shift) |
			(b << color_b_shift) |
			(a << color_a_shift)
		);
	}
}

#endif /* color_hpp */
