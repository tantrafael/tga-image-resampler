#ifndef resampler_helper_hpp
#define resampler_helper_hpp

#include <math.h>

#include "kernel_direction.hpp"

namespace tga
{
	#define BLOCK_OFFSET_RGB32(ptr, width, x, y) (ptr + (4 * width) * y + 4 * x)

	inline int32_t clipRange(int32_t input, int32_t low, int32_t high)
	{
		return (input < low) ? low : (input > high) ? high : input;
	}

	class ResamplerHelper
	{
	public:
		static bool getSourcePixel(const float subPixelPosX,
								   const float subPixelPosY,
								   const KernelDirection direction,
								   const int offset,
								   uint8_t* const pixels,
								   const int32_t width,
								   const int32_t height,
								   float& distance,
								   uint8_t*& sourcePixel);

		static void accumulateSamples(const uint8_t* const sourcePixel,
									  const float weight,
									  float (&totalSamples)[3],
									  float& sampleCount);
	};
}

#endif /* resampler_helper_hpp */
