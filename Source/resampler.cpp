#include "resampler.hpp"

//#include <unordered_map>
#include <map>

#include "helper.hpp"

namespace tga
{
	Resampler::Resampler()
	{}

	/*
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

		std::shared_ptr<KernelSampler> sampler = KernelSampler::create(type);

		resampleDirection(sampler,
						  Horizontal,
						  mappingRatioX,
						  mappingRatioY,
						  sourceHeader.width,
						  sourceHeader.height,
						  sourceImage.pixels,
						  targetHeader.width,
						  sourceHeader.height,
						  bufferPixels);

		resampleDirection(sampler,
						  Vertical,
						  mappingRatioX,
						  mappingRatioY,
						  targetHeader.width,
						  sourceHeader.height,
						  bufferPixels,
						  targetHeader.width,
						  targetHeader.height,
						  targetImage.pixels);

		return true;
	}
	*/

	bool Resampler::resample(const Header& sourceHeader,
							 const Image& sourceImage,
							 const int targetWidth,
							 const int targetHeight,
							 const KernelType type,
							 Header& targetHeader,
							 Image& targetImage)
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
		targetHeader.width = targetWidth;
		targetHeader.height = targetHeight;
		targetHeader.pixelBitDepth = sourceHeader.pixelBitDepth;
		targetHeader.imageDescriptor = sourceHeader.imageDescriptor;
		targetHeader.imageId = sourceHeader.imageId;
		targetHeader.colorMap = sourceHeader.colorMap;

		// Image
		targetImage.pixelByteDepth = sourceImage.pixelByteDepth;
		targetImage.rowStride = targetHeader.width * targetImage.pixelByteDepth;

		const auto targetBufferSize{ targetImage.rowStride * targetHeader.height };
		std::vector<uint8_t> targetBuffer(targetBufferSize);
		targetImage.pixels = targetBuffer.data();

		// Allocate a temporary buffer to hold our horizontal pass output.
		// We're using unique_ptr rather than vector because we want a fast and
		// smart way to allocate very large buffers without initialization.
		const auto tempBufferSize{ targetImage.rowStride * sourceHeader.height };
		std::unique_ptr<uint8_t[]> tempBuffer(new uint8_t[tempBufferSize]);
		auto tempBufferPixels{ tempBuffer.get() };

		const auto sourceMappingWidth{ static_cast<float>(sourceHeader.width - 1) };
		const auto targetMappingWidth{ static_cast<float>(targetHeader.width - 1) };
		const auto mappingRatioX = sourceMappingWidth / targetMappingWidth;

		const auto sourceMappingHeight{ static_cast<float>(sourceHeader.height - 1) };
		const auto targetMappingHeight{ static_cast<float>(targetHeader.height - 1) };
		const auto mappingRatioY = sourceMappingHeight / targetMappingHeight;

		std::shared_ptr<KernelSampler> sampler = KernelSampler::create(type);

		resampleDirection(sampler,
						  Horizontal,
						  mappingRatioX,
						  mappingRatioY,
						  sourceHeader.width,
						  sourceHeader.height,
						  sourceImage.pixels,
						  targetHeader.width,
						  sourceHeader.height,
						  tempBufferPixels);

		resampleDirection(sampler,
						  Vertical,
						  mappingRatioX,
						  mappingRatioY,
						  targetHeader.width,
						  sourceHeader.height,
						  tempBufferPixels,
						  targetHeader.width,
						  targetHeader.height,
						  targetImage.pixels);

		return true;
	}

	bool Resampler::resampleDirection(std::shared_ptr<KernelSampler> sampler,
									  const KernelDirection direction,
									  const float mappingRatioX,
									  const float mappingRatioY,
									  const int inputWidth,
									  const int inputHeight,
									  uint8_t* const inputPixels,
									  const int outputWidth,
									  const int outputHeight,
									  uint8_t* const outputPixels)
	{
		for (int outputRow = 0; outputRow < outputHeight; ++outputRow)
		{
			for (int outputCol = 0; outputCol < outputWidth; ++outputCol)
			{
				resamplePosition(sampler,
								 direction,
								 mappingRatioX,
								 mappingRatioY,
								 outputRow,
								 outputCol,
								 inputPixels,
								 inputWidth,
								 inputHeight,
								 outputWidth,
								 outputPixels);
			}
		}

		return true;
	}

	bool Resampler::resamplePosition(std::shared_ptr<KernelSampler> sampler,
									 const KernelDirection direction,
									 const float mappingRatioX,
									 const float mappingRatioY,
									 const int outputRow,
									 const int outputCol,
									 uint8_t* const inputPixels,
									 const int inputWidth,
									 const int inputHeight,
									 const int outputWidth,
									 uint8_t* const outputPixels)
	{
		// Determine the sub-pixel location of our target (col, row)
		// coordinate, in the space of our source image.
		float subPixelPosX{};
		float subPixelPosY{};

		if (direction == Horizontal)
		{
			subPixelPosX = static_cast<float>(outputCol * mappingRatioX);
			subPixelPosY = static_cast<float>(outputRow);
		}
		else if (direction == Vertical)
		{
			subPixelPosX = static_cast<float>(outputCol);
			subPixelPosY = static_cast<float>(outputRow * mappingRatioY);
		}

		// Sample the source image at the sub-pixel location, using the
		// specified algorithm in the the current direction.
		float sampleCount{ 0 };
		float totalSamples[3]{0, 0, 0};

		sampler->sample(subPixelPosX,
						subPixelPosY,
						direction,
						inputPixels,
						inputWidth,
						inputHeight,
						sampleCount,
						totalSamples);

		// Normalize our sum back to the valid pixel range.
		const float scaleFactor = 1.0f / sampleCount;
		uint8_t* output = BLOCK_OFFSET_RGB32(outputPixels, outputWidth, outputCol, outputRow);

		output[0] = clipRange(scaleFactor * totalSamples[0], 0, 255);
		output[1] = clipRange(scaleFactor * totalSamples[1], 0, 255);
		output[2] = clipRange(scaleFactor * totalSamples[2], 0, 255);

		return true;
	}
}
