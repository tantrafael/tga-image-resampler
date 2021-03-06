#include "lanczos_sampler.hpp"

#include <cmath>

namespace tga
{
	constexpr auto pi{ 3.14159265358979323846 };

	inline float sinc(const float x)
	{
		if (x == 0.0f)
		{
			return 1.0f;
		}

		return std::sin(pi * x) / (pi * x);
	}

	inline float lanczosWeight(const float coeffA, const float distance)
	{
		if (distance <= coeffA)
		{
			return sinc(distance) * sinc(distance / coeffA);
		}

		return 0.0f;
	}

	LanczosSampler::LanczosSampler(const float coeffA)
		: m_coeffA{ coeffA }
	{}

	bool LanczosSampler::sample(const float subPixelPosX,
								const float subPixelPosY,
								const ResamplingDirection direction,
								uint8_t* pixels,
								const int width,
								const int height,
								float& sampleCount,
								float (&totalSamples)[3])
	{
		const bool isValidInput = (subPixelPosX >= 0.0f
								   && subPixelPosY >= 0.0f
								   && pixels != nullptr
								   && width > 0
								   && height > 0);

		if (!isValidInput)
		{
			return false;
		}

		int32_t radius = m_coeffA;

		for (int offset = -radius; offset < radius; ++offset)
		{
			float distance{};
			uint8_t* sourcePixel{};

			if (ResamplerHelper::getSourcePixel(subPixelPosX,
												subPixelPosY,
												direction,
												offset,
												pixels,
												width,
												height,
												distance,
												sourcePixel))
			{
				const auto weight{ lanczosWeight(m_coeffA, distance) };

				ResamplerHelper::accumulateSamples(sourcePixel,
												   weight,
												   totalSamples,
												   sampleCount);
			}
		}

		return true;
	}
}
