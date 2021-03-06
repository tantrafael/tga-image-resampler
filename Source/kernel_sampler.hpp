#ifndef kernel_sampler_hpp
#define kernel_sampler_hpp

#include <memory>

#include "kernel_type.hpp"
#include "resampling_direction.hpp"

namespace tga
{
	class KernelSampler
	{
	public:
		static std::shared_ptr<KernelSampler> create(const KernelType kernel);

		virtual bool sample(const float subPixelPosX,
							const float subPixelPosY,
							const ResamplingDirection direction,
							uint8_t* pixels,
							const int width,
							const int height,
							float& sampleCount,
							float (&totalSamples)[3]) = 0;

		virtual ~KernelSampler() = 0;
	};
}

#endif /* kernel_sampler_hpp */
