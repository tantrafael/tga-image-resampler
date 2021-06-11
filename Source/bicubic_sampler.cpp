#include "bicubic_sampler.hpp"

namespace tga
{
	inline float bicubicWeight(const float coeffB,
							   const float coeffC,
							   const float distance)
	{
		// Our bicubic function is designed to provide feedback over a radius
		// of 2.0 pixels.
		float distance2 = distance * distance;
		float distance3 = distance * distance * distance;
		float result = 0.0f;

		if (distance < 1.0f)
		{
			float cubicTerm = (12.0f - 9.0f * coeffB - 6.0f * coeffC) * distance3;
			float quadTerm = (-18.0f + 12.0f * coeffB + 6.0f * coeffC) * distance2;
			float constTerm = (6.0f - 2.0f * coeffB);
			result = (1.0f / 6.0f) * (cubicTerm + quadTerm + constTerm);
		}
		else if (distance >= 1.0f && distance < 2.0f)
		{
			float cubicTerm = (-coeffB - 6.0f * coeffC) * distance3;
			float quadTerm = (6.0f * coeffB + 30.0f * coeffC) * distance2;
			float linTerm = (-12.0f * coeffB - 48.0f * coeffC) * distance;
			float constTerm = (8.0f * coeffB + 24.0f * coeffC);
			result = (1.0f / 6.0f) * (cubicTerm + quadTerm + linTerm + constTerm);
		}

		if (result < 0.0f)
		{
			result = 0.0;
		}

		return result;
	}

	BicubicSampler::BicubicSampler(const float coeffB, const float coeffC)
		: m_coeffB{ coeffB }
		, m_coeffC{ coeffC }
	{}

	bool BicubicSampler::sample(const float subPixelPosX,
								const float subPixelPosY,
								const ResamplingDirection direction,
								uint8_t* pixels,
								const unsigned int width,
								const unsigned int height,
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

		for (int offset = -2; offset < 2; ++offset)
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
				const auto weight{ bicubicWeight(m_coeffB, m_coeffC, distance) };

				ResamplerHelper::accumulateSamples(sourcePixel,
												   weight,
												   totalSamples,
												   sampleCount);
			}
		}

		return true;
	}
}
