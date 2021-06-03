#ifndef baz_hpp
#define baz_hpp

#include "bar.hpp"

namespace tga
{
	inline float sinc(const float x)
	{
		if (x == 0.0f)
		{
			return 1.0f;
		}

		return sin(M_PI * x) / (M_PI * x);
	}

	inline float lanczosWeight(const float coeffA, const float distance)
	{
		if (distance <= coeffA)
		{
			return sinc(distance) * sinc(distance / coeffA);
		}

		return 0.0f;
	}

	class LanczosSampler
	{
	public:
		LanczosSampler(const float coeffA)
			: m_coeffA{ coeffA }
		{}

		int operator() (const float subPixelPosX,
						const float subPixelPosY,
						const KernelDirection direction,
						uint8_t* pixels,
						const int32_t width,
						const int32_t height,
						float& sampleCount,
						float (&totalSamples)[3])
		{
			const bool isValidInput = (subPixelPosX >= 0.0f
									   && subPixelPosY >= 0.0f
									   && pixels != nullptr
									   && width >= 0
									   && height >= 0);

			if (!isValidInput)
			{
				return false;
			}

			int32_t radius = m_coeffA;

			for (int offset = -radius; offset < radius; ++offset)
			{
				float distance{};
				uint8_t* sourcePixel{};

				if (!Bar::getSourcePixel(subPixelPosX,
										 subPixelPosY,
										 direction,
										 offset,
										 pixels,
										 width,
										 height,
										 distance,
										 sourcePixel))
				{
					continue;
				}

				const auto weight{ lanczosWeight(m_coeffA, distance) };
				Bar::accumulateSamples(sourcePixel, weight, totalSamples, sampleCount);
			}

			return true;
		}

	private:
		float m_coeffA;
	};
}

#endif /* baz_hpp */
