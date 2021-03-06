#ifndef lanczos_sampler_hpp
#define lanczos_sampler_hpp

#include "kernel_sampler.hpp"
#include "resampler_helper.hpp"

namespace tga
{
	class LanczosSampler : public KernelSampler
	{
	public:
		LanczosSampler(const float coeffA);

		bool sample(const float subPixelPosX,
					const float subPixelPosY,
					const ResamplingDirection direction,
					uint8_t* pixels,
					const int width,
					const int height,
					float& sampleCount,
					float (&totalSamples)[3]);

	private:
		float m_coeffA;
	};
}

#endif /* lanczos_sampler_hpp */
