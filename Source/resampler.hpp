#ifndef resampler_hpp
#define resampler_hpp

#include "image.hpp"
#include "kernel_type.hpp"
#include "resampling_direction.hpp"
#include "kernel_sampler.hpp"

namespace tga
{
	class Resampler
	{
	public:
		Resampler();

		bool resample(const Image& sourceImage,
					  const int destinationWidth,
					  const int destinationHeight,
					  const KernelType kernel,
					  Image& destinationImage);

	private:
		bool resampleHeader(const Image& sourceImage,
							const int destinationWidth,
							const int destinationHeight,
							Image& destinationImage);

		bool resampleBody(const Image& sourceImage,
						  const KernelType kernel,
						  Image& destinationImage);

		bool resampleDirection(const std::shared_ptr<KernelSampler> sampler,
							   const ResamplingDirection direction,
							   const float mappingRatioX,
							   const float mappingRatioY,
							   const int inputWidth,
							   const int inputHeight,
							   uint8_t* const inputPixels,
							   const int outputWidth,
							   const int outputHeight,
							   uint8_t* const outputPixels);

		bool resamplePosition(const std::shared_ptr<KernelSampler> sampler,
							  const ResamplingDirection direction,
							  const float mappingRatioX,
							  const float mappingRatioY,
							  const int outputRow,
							  const int outputCol,
							  uint8_t* const inputPixels,
							  const int inputWidth,
							  const int inputHeight,
							  const int outputWidth,
							  uint8_t* const outputPixels);
	};
}

#endif /* resampler_hpp */
