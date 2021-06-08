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

		bool resample(const Header& sourceHeader,
					  const Image& sourceImage,
					  Header& targetHeader,
					  Image& targetImage,
					  KernelType type);

		static bool getSourcePixel(const float subPixelPosX,
								   const float subPixelPosY,
								   const KernelDirection direction,
								   const int offset,
								   uint8_t* pixels,
								   const int32_t width,
								   const int32_t height,
								   float& distance,
								   uint8_t*& sourcePixel);

	private:
		bool resampleDirection(std::shared_ptr<KernelSampler> sampler,
							   const KernelDirection direction,
							   const int inputWidth,
							   const int inputHeight,
							   uint8_t* inputPixels,
							   const int outputWidth,
							   const int outputHeight,
							   uint8_t* outputPixels,
							   const float mappingRatioX,
							   const float mappingRatioY);

		/*
		bool resamplePosition(std::shared_ptr<KernelSampler> sampler,
							  KernelDirection direction,
							  uint8_t* pixels,
							  uint32_t width,
							  uint32_t height,
							  float subPixelPosX,
							  float subPixelPosY,
							  const float mappingRatioX,
							  const float mappingRatioY,
							  uint8_t* output);
		*/

		bool resamplePosition(std::shared_ptr<KernelSampler> sampler,
				 const KernelDirection direction,
				 const int outputRow,
				 const int outputCol,
				 const float mappingRatioX,
				 const float mappingRatioY,
				 uint8_t* inputPixels,
				 const int inputWidth,
				 const int inputHeight,
				 const int outputWidth,
				 uint8_t* outputPixels);
	};
}

#endif /* resampler_hpp */
