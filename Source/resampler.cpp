#include "resampler.hpp"

//#include <unordered_map>
#include <map>

#include "helper.hpp"
#include "kernel_sampler_factory.hpp"
/*
#include "bicubic.hpp"
#include "lanczos.hpp"
*/

namespace tga
{
	/*
	template<typename T>
	struct Wrapper
	{
		typedef std::map<KernelType, std::function<T()>> map_type;
	};
	*/

	Resampler::Resampler()
	{}

	bool Resampler::resample(const Header& sourceHeader,
							 const Image& sourceImage,
							 Header& targetHeader,
							 Image& targetImage,
							 KernelType type)
	{
		// Header
		targetHeader.idLength = sourceHeader.idLength;
		targetHeader.colorMapType = sourceHeader.colorMapType;
		targetHeader.imageType = sourceHeader.imageType;
		targetHeader.colorMapOrigin = sourceHeader.colorMapOrigin;
		targetHeader.colorMapLength = sourceHeader.colorMapLength;
		targetHeader.colorMapBitDepth = sourceHeader.colorMapBitDepth;
		targetHeader.xOrigin = sourceHeader.xOrigin;
		targetHeader.yOrigin = sourceHeader.yOrigin;
		targetHeader.pixelBitDepth = sourceHeader.pixelBitDepth;
		targetHeader.imageDescriptor = sourceHeader.imageDescriptor;
		targetHeader.imageId = sourceHeader.imageId;
		targetHeader.colorMap = sourceHeader.colorMap;

		// Image
		targetImage.pixelByteDepth = sourceImage.pixelByteDepth;
		//targetImage.rowStride = targetHeader.width * targetImage.pixelByteDepth;

		// Allocate a temporary buffer to hold our horizontal pass output.
		// We're using unique_ptr rather than vector because we want a fast and
		// smart way to allocate very large buffers without initialization.
		const auto bufferSize{ targetImage.rowStride * sourceHeader.height };
		std::unique_ptr<uint8_t[]> buffer(new uint8_t[bufferSize]);
		auto bufferPixels{ buffer.get() };

		const auto sourceMappingWidth{ static_cast<float>(sourceHeader.width - 1) };
		const auto targetMappingWidth{ static_cast<float>(targetHeader.width - 1) };
		const auto mappingRatioX = sourceMappingWidth / targetMappingWidth;

		const auto sourceMappingHeight{ static_cast<float>(sourceHeader.height - 1) };
		const auto targetMappingHeight{ static_cast<float>(targetHeader.height - 1) };
		const auto mappingRatioY = sourceMappingHeight / targetMappingHeight;

		//KernelSampler* sampler = KernelSamplerFactory::create(type);
		//KernelSampler* sampler = KernelSampler(type);
		KernelSampler* sampler = KernelSampler::create(type);
		//KernelSampler* sampler = new BicubicSampler{ 0.0f, 1.0f };

		resampleDirection(sampler,
						  Horizontal,
						  sourceHeader.width,
						  sourceHeader.height,
						  sourceImage.pixels,
						  targetHeader.width,
						  sourceHeader.height,
						  bufferPixels,
						  mappingRatioX,
						  mappingRatioY);

		resampleDirection(sampler,
						  Vertical,
						  targetHeader.width,
						  sourceHeader.height,
						  bufferPixels,
						  targetHeader.width,
						  targetHeader.height,
						  targetImage.pixels,
						  mappingRatioX,
						  mappingRatioY);

		return true;
	}

	//template<typename T>
	//bool Resampler::resampleDirection(T& sample,
	bool Resampler::resampleDirection(KernelSampler* sampler,
									  const KernelDirection direction,
									  const int inputWidth,
									  const int inputHeight,
									  uint8_t* inputPixels,
									  const int outputWidth,
									  const int outputHeight,
									  uint8_t* outputPixels,
									  const float mappingRatioX,
									  const float mappingRatioY)
	{
		for (int row = 0; row < outputHeight; ++row)
		{
			for (int col = 0; col < outputWidth; ++col)
			{
				uint8_t* output = BLOCK_OFFSET_RGB32(outputPixels, outputWidth, col, row);

				// Determine the sub-pixel location of our target (col, row)
				// coordinate, in the space of our source image.
				float subPixelPosX{};
				float subPixelPosY{};

				if (direction == Horizontal)
				{
					subPixelPosX = static_cast<float>(col * mappingRatioX);
					subPixelPosY = static_cast<float>(row);
				}
				else if (direction == Vertical)
				{
					subPixelPosX = static_cast<float>(col);
					subPixelPosY = static_cast<float>(row * mappingRatioY);
				}

				//sampleKernel(sample,
				sampleKernel(sampler,
							 direction,
							 inputPixels,
							 inputWidth,
							 inputHeight,
							 subPixelPosX,
							 subPixelPosY,
							 mappingRatioX,
							 mappingRatioY,
							 output);
			}
		}

		return true;
	}

	//template<typename T>
	//bool Resampler::sampleKernel(T& sample,
	bool Resampler::sampleKernel(KernelSampler* sampler,
								 KernelDirection direction,
								 uint8_t* pixels,
								 uint32_t width,
								 uint32_t height,
								 float subPixelPosX,
								 float subPixelPosY,
								 float mappingRatioX,
								 float mappingRatioY,
								 uint8_t* output)
	{
		const bool isValidInput = (pixels != nullptr
								   && width >= 0
								   && height >= 0
								   && subPixelPosX >= 0.0f
								   && subPixelPosY >= 0.0f
								   && output != nullptr);

		if (!isValidInput)
		{
			return false;
		}

		// TODO: List initialize.
		float sampleCount = 0;
		float totalSamples[3] = {0};

		/*
		sample(subPixelPosX,
			   subPixelPosY,
			   direction,
			   pixels,
			   width,
			   height,
			   sampleCount,
			   totalSamples);
		*/

		sampler->sample(subPixelPosX,
						subPixelPosY,
						direction,
						pixels,
						width,
						height,
						sampleCount,
						totalSamples);

		// Normalize our sum back to the valid pixel range.
		float scaleFactor = 1.0f / sampleCount;
		output[0] = clipRange(scaleFactor * totalSamples[0], 0, 255);
		output[1] = clipRange(scaleFactor * totalSamples[1], 0, 255);
		output[2] = clipRange(scaleFactor * totalSamples[2], 0, 255);

		return true;
	}
}
