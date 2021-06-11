#ifndef resampler_hpp
#define resampler_hpp

#include <math.h>

#include "image.hpp"
#include "kernel_type.hpp"
#include "kernel_direction.hpp"
#include "kernel_sampler.hpp"

namespace tga
{
	class Resampler
	{
	public:
		Resampler();

		bool resample(const Image& sourceImage,
					  const unsigned int destinationWidth,
					  const unsigned int destinationHeight,
					  const KernelType kernel,
					  Image& destinationImage);

	private:
		void resampleHeader(const Image& sourceImage,
							const unsigned int destinationWidth,
							const unsigned int destinationHeight,
							Image& destinationImage);

		void resampleBody(const Image& sourceImage,
						  const KernelType kernel,
						  Image& destinationImage);

		bool resampleDirection(const std::shared_ptr<KernelSampler> sampler,
							   const KernelDirection direction,
							   const float mappingRatioX,
							   const float mappingRatioY,
							   const unsigned int inputWidth,
							   const unsigned int inputHeight,
							   uint8_t* const inputPixels,
							   const unsigned int outputWidth,
							   const unsigned int outputHeight,
							   uint8_t* const outputPixels);

		bool resamplePosition(const std::shared_ptr<KernelSampler> sampler,
							  const KernelDirection direction,
							  const float mappingRatioX,
							  const float mappingRatioY,
							  const unsigned int outputRow,
							  const unsigned int outputCol,
							  uint8_t* const inputPixels,
							  const unsigned int inputWidth,
							  const unsigned int inputHeight,
							  const unsigned int outputWidth,
							  uint8_t* const outputPixels);
	};
}

#endif /* resampler_hpp */
