#ifndef bicubic_sampler_hpp
#define bicubic_sampler_hpp

#include "kernel_sampler.hpp"
#include "resampler_helper.hpp"

namespace tga
{
	class BicubicSampler : public KernelSampler
	{
	public:
		BicubicSampler(const float coeffB, const float coeffC);

		bool sample(const float subPixelPosX,
					const float subPixelPosY,
					const ResamplingDirection direction,
					uint8_t* pixels,
					const unsigned int width,
					const unsigned int height,
					float& sampleCount,
					float (&totalSamples)[3]);

	private:
		float m_coeffB;
		float m_coeffC;
	};
}

#endif /* bicubic_sampler_hpp */
