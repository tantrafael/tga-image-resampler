#ifndef resampler_hpp
#define resampler_hpp

#include <math.h>

/*
#include "image_header.hpp"
#include "image_body.hpp"
*/
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
					  const int targetWidth,
					  const int targetHeight,
					  const KernelType type,
					  Image& targetImage);

	private:
		void resampleHeader(const Image& sourceImage,
							const int targetWidth,
							const int targetHeight,
							Image& targetImage);

		void resampleBody(const Image& sourceImage,
						  const KernelType type,
						  Image& targetImage);

		bool resampleDirection(const std::shared_ptr<KernelSampler> sampler,
							   const KernelDirection direction,
							   const float mappingRatioX,
							   const float mappingRatioY,
							   const int inputWidth,
							   const int inputHeight,
							   uint8_t* const inputPixels,
							   const int outputWidth,
							   const int outputHeight,
							   uint8_t* const outputPixels);

		bool resamplePosition(const std::shared_ptr<KernelSampler> sampler,
							  const KernelDirection direction,
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
