#ifndef bar_hpp
#define bar_hpp

#include <cmath>

#include "kernel_direction.hpp"

namespace tga
{
	#define BLOCK_OFFSET_RGB32(ptr, width, x, y) (ptr + (4 * width) * y + 4 * x)

	inline int32_t clipRange(int32_t input, int32_t low, int32_t high)
	{
		return (input < low) ? low : (input > high) ? high : input;
	}

	class Bar
	{
	public:
		static bool getSourcePixel(const float subPixelPosX,
								   const float subPixelPosY,
								   const KernelDirection direction,
								   const int offset,
								   uint8_t* pixels,
								   const int32_t width,
								   const int32_t height,
								   float& distance,
								   uint8_t*& sourcePixel)
		{
			int32_t samplePosX{};
			int32_t samplePosY{};
			float delta{};

			if (direction == Horizontal)
			{
				samplePosX = static_cast<int32_t>(subPixelPosX + offset);
				samplePosY = static_cast<int32_t>(subPixelPosY);
				delta = static_cast<float>(subPixelPosX - samplePosX);
			}
			else if (direction == Vertical)
			{
				samplePosX = static_cast<int32_t>(subPixelPosX);
				samplePosY = static_cast<int32_t>(subPixelPosY + offset);
				delta = static_cast<float>(subPixelPosY - samplePosY);
			}

			const bool isValidSamplePos = (samplePosX >= 0
										   && samplePosY >= 0
										   && samplePosX < width
										   && samplePosY < height);

			if (!isValidSamplePos)
			{
				return false;
			}

			distance = fabs(delta);
			sourcePixel = BLOCK_OFFSET_RGB32(pixels, width, samplePosX, samplePosY);

			return true;
		}

		static void accumulateSamples(const uint8_t* sourcePixel,
									  const float weight,
									  float (&totalSamples)[3],
									  float& sampleCount)
		{
			// Accumulate weighted samples from the source.
			totalSamples[0] += sourcePixel[0] * weight;
			totalSamples[1] += sourcePixel[1] * weight;
			totalSamples[2] += sourcePixel[2] * weight;

			// Record the total weights of the sample for later normalization.
			sampleCount += weight;
		}
	};
}

#endif /* bar_hpp */
