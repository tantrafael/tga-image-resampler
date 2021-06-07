#ifndef kernel_sampler_hpp
#define kernel_sampler_hpp

#include "kernel_direction.hpp"

namespace tga
{
	// TODO: Add virtual destructor.
	class KernelSampler
	{
	public:
		/*
		virtual bool operator() (const float subPixelPosX,
								 const float subPixelPosY,
								 const KernelDirection direction,
								 uint8_t* pixels,
								 const int32_t width,
								 const int32_t height,
								 float& sampleCount,
								 float (&totalSamples)[3]) = 0;
		*/

		virtual bool sample(const float subPixelPosX,
							const float subPixelPosY,
							const KernelDirection direction,
							uint8_t* pixels,
							const int32_t width,
							const int32_t height,
							float& sampleCount,
							float (&totalSamples)[3]) = 0;
	};
}

#endif /* kernel_sampler_hpp */
