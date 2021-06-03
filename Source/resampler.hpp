#ifndef resampler_hpp
#define resampler_hpp

#include <math.h>

#include "header.hpp"
#include "image.hpp"
#include "kernel_type.hpp"
#include "kernel_direction.hpp"
#include "foo.hpp"
#include "baz.hpp"

namespace tga
{
	// TODO: Move to Lanczos implementation.
	/*
	inline float sinc(const float x)
	{
		if (x == 0.0f)
		{
			return 1.0f;
		}

		return sin(M_PI * x) / (M_PI * x);
	}

	inline float lanczosWeight(const float coeffA, const float distance)
	{
		if (distance <= coeffA)
		{
			return sinc(distance) * sinc(distance / coeffA);
		}

		return 0.0f;
	}
	*/

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
		/*
		bool resampleDirection(BicubicSampler& sampler,
							   const KernelDirection direction,
							   const int inputWidth,
							   const int inputHeight,
							   uint8_t* inputPixels,
							   const int outputWidth,
							   const int outputHeight,
							   uint8_t* outputPixels,
							   const float mappingRatioX,
							   const float mappingRatioY);
		*/

		template<typename T>
		bool resampleDirection(T& sample,
							   const KernelDirection direction,
							   const int inputWidth,
							   const int inputHeight,
							   uint8_t* inputPixels,
							   const int outputWidth,
							   const int outputHeight,
							   uint8_t* outputPixels,
							   const float mappingRatioX,
							   const float mappingRatioY);

		template<typename T>
		bool sampleKernel(T& sample,
						  KernelDirection direction,
						  uint8_t* pixels,
						  uint32_t width,
						  uint32_t height,
						  float subPixelPosX,
						  float subPixelPosY,
						  const float mappingRatioX,
						  const float mappingRatioY,
						  uint8_t* output);

		/*
		bool sampleKernelBicubic(const float subPixelPosX,
								 const float subPixelPosY,
								 const KernelDirection direction,
								 uint8_t* pixels,
								 const int32_t width,
								 const int32_t height,
								 float& sampleCount,
								 float (&totalSamples)[3]);
		*/

		/*
		bool getSourcePixel(const float subPixelPosX,
							const float subPixelPosY,
							const KernelDirection direction,
							const int offset,
							uint8_t* pixels,
							const int32_t width,
							const int32_t height,
							float& distance,
							uint8_t*& sourcePixel);
		*/

		/*
		void accumulateSamples(const uint8_t* sourcePixel,
							   const float weight,
							   float (&totalSamples)[3],
							   float& sampleCount);
		*/

		/*
		bool sampleKernel(uint8_t* pixels,
						  uint32_t width,
						  uint32_t height,
						  KernelDirection direction,
						  float subPixelPosX,
						  float subPixelPosY,
						  KernelType type,
						  float mappingRatioX,
						  float mappingRatioY,
						  uint8_t* output);
		*/

		/*
		bool sampleKernel(foo,
						  KernelDirection direction,
						  uint8_t* pixels,
						  uint32_t width,
						  uint32_t height,
						  float subPixelPosX,
						  float subPixelPosY,
						  float mappingRatioX,
						  float mappingRatioY,
						  uint8_t* output);

		bool sampleKernelBicubic(uint8_t* pixels,
								 uint32_t width,
								 uint32_t height,
								 KernelDirection direction,
								 float subPixelPosX,
								 float subPixelPosY,
								 float coeffB,
								 float coeffC,
								 uint8_t* output);

		bool sampleKernelLanczos(uint8_t* pixels,
								 uint32_t width,
								 uint32_t height,
								 KernelDirection direction,
								 float subPixelPosX,
								 float subPixelPosY,
								 float coeffA,
								 uint8_t* output);
		*/
	};
}

#endif /* resampler_hpp */
