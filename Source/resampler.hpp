#ifndef resampler_hpp
#define resampler_hpp

#include <math.h>

#include "header.hpp"
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

		/*
		bool resample(const Header& sourceHeader,
					  const Image& sourceImage,
					  Header& targetHeader,
					  Image& targetImage,
					  KernelType type);
		*/

		bool resample(const Header& sourceHeader,
					  const Image& sourceImage,
					  const int width,
					  const int height,
					  const KernelType type,
					  Header& targetHeader,
					  Image& targetImage);

		/*
		static bool getSourcePixel(const float subPixelPosX,
								   const float subPixelPosY,
								   const KernelDirection direction,
								   const int offset,
								   uint8_t* pixels,
								   const int32_t width,
								   const int32_t height,
								   float& distance,
								   uint8_t*& sourcePixel);
		*/

	private:
		bool resampleDirection(std::shared_ptr<KernelSampler> sampler,
							   const KernelDirection direction,
							   const float mappingRatioX,
							   const float mappingRatioY,
							   const int inputWidth,
							   const int inputHeight,
							   uint8_t* const inputPixels,
							   const int outputWidth,
							   const int outputHeight,
							   uint8_t* const outputPixels);

		bool resamplePosition(std::shared_ptr<KernelSampler> sampler,
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
