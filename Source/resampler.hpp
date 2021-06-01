#ifndef resampler_hpp
#define resampler_hpp

#include <math.h>

#include "header.hpp"
#include "image.hpp"
#include "kernel_type.hpp"
#include "kernel_direction.hpp"

namespace tga
{
	#define BLOCK_OFFSET_RGB32(ptr, width, x, y) (ptr + (4 * width) * y + 4 * x)

	inline int32_t clipRange(int32_t input, int32_t low, int32_t high)
	{
		return (input < low) ? low : (input > high) ? high : input;
	}

	inline float bicubicWeight(const float coeffB,
							   const float coeffC,
							   const float distance)
	{
		// Our bicubic function is designed to provide feedback over a radius
		// of 2.0 pixels.
		float distance2 = distance * distance;
		float distance3 = distance * distance * distance;
		float result = 0.0f;

		if (distance < 1.0f)
		{
			float cubicTerm = (12.0f - 9.0f * coeffB - 6.0f * coeffC) * distance3;
			float quadTerm = (-18.0f + 12.0f * coeffB + 6.0f * coeffC) * distance2;
			float constTerm = (6.0f - 2.0f * coeffB);
			result = (1.0f / 6.0f) * (cubicTerm + quadTerm + constTerm);
		}
		else if (distance >= 1.0f && distance < 2.0f)
		{
			float cubicTerm = (-coeffB - 6.0f * coeffC) * distance3;
			float quadTerm = (6.0f * coeffB + 30.0f * coeffC) * distance2;
			float linTerm = (-12.0f * coeffB - 48.0f * coeffC) * distance;
			float constTerm = (8.0f * coeffB + 24.0f * coeffC);
			result = (1.0f / 6.0f) * (cubicTerm + quadTerm + linTerm + constTerm);
		}

		if (result < 0.0f)
		{
			result = 0.0;
		}

		return result;
	}

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

	class Resampler
	{
	public:
		Resampler();

		bool resample(const Header& sourceHeader,
					  const Image& sourceImage,
					  Header& targetHeader,
					  Image& targetImage,
					  KernelType type);

	private:
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

		bool foo(KernelDirection direction,
				 uint8_t* pixels,
				 uint32_t width,
				 uint32_t height,
				 float subPixelPosX,
				 float subPixelPosY,
				 float mappingRatioX,
				 float mappingRatioY,
				 uint8_t* output);

		bool bar(const int i,
				 const KernelDirection direction,
				 uint8_t* pixels,
				 const int32_t width,
				 const int32_t height,
				 const float subPixelPosX,
				 const float subPixelPosY,
				 float &distance,
				 uint8_t* &sourcePixel);
	};
}

#endif /* resampler_hpp */
