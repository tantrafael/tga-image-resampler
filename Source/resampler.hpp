#ifndef resampler_hpp
#define resampler_hpp

#include "header.hpp"
#include "image.hpp"

namespace tga
{
	#define BLOCK_OFFSET_RGB24(ptr, width, x, y) (ptr + (4 * width) * y + 4 * x)

	inline int32_t clipRange(int32_t input, int32_t low, int32_t high)
	{
		return (input < low) ? low : (input > high) ? high : input;
	}

	inline float_t bicubicWeight(float_t f_b, float_t f_c, float_t distance)
	{
		/* Our bicubic function is designed to provide feedback over a radius of 2.0
		* pixels. */
		float_t distance2 = distance * distance;
		float_t distance3 = distance * distance * distance;
		float_t result = 0.0;

		if (distance < 1.0)
		{
			float_t cubic_term = (12.0 - 9.0 * f_b - 6.0 * f_c) * distance3;
			float_t quad_term = (-18.0 + 12.0 * f_b + 6.0 * f_c) * distance2;
			float_t const_term = (6.0 - 2.0 * f_b);
			result = (1.0f / 6.0f) * (cubic_term + quad_term + const_term);
		}
		else if (distance >= 1.0 && distance < 2.0)
		{
			float_t cubic_term = (-f_b - 6.0 * f_c) * distance3;
			float_t quad_term = (6.0 * f_b + 30.0 * f_c) * distance2;
			float_t lin_term = (-12.0 * f_b - 48.0 * f_c) * distance;
			float_t const_term = (8.0 * f_b + 24.0 * f_c);
			result = (1.0f / 6.0f) * (cubic_term + quad_term + lin_term + const_term);
		}

		if (result < 0)
		{
			result = 0.0;
		}

		return result;
	}

	class Resampler
	{
	public:
		Resampler();


		bool resample(const Header& sourceHeader,
					  const Image& sourceImage,
					  Header& targetHeader,
					  Image& targetImage);

	private:
		bool sampleKernelBicubicH(uint8_t* sourcePixels,
								  uint32_t sourceWidth,
								  uint32_t sourceHeight,
								  float_t f_x,
								  float_t f_y,
								  uint8_t* output);

		bool sampleKernelBicubicV(uint8_t* sourcePixels,
								  uint32_t sourceWidth,
								  uint32_t sourceHeight,
								  float_t f_x,
								  float_t f_y,
								  uint8_t* output);
	};
}

#endif /* resampler_hpp */
