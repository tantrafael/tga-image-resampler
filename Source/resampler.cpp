#include "resampler.hpp"

#include "helper.hpp"

namespace tga
{
	Resampler::Resampler()
	{}

	/*
	bool Resampler::resample(const ImageHeader& sourceHeader,
							 const ImageBody& sourceImage,
							 const int targetWidth,
							 const int targetHeight,
							 const KernelType type,
							 ImageHeader& targetHeader,
							 ImageBody& targetImage)
	*/

	bool Resampler::resample(const Image& sourceImage,
							 const int targetWidth,
							 const int targetHeight,
							 const KernelType type,
							 Image& targetImage)
	{
		// Header
		targetImage.header.idLength = sourceImage.header.idLength;
		targetImage.header.colorMapType = sourceImage.header.colorMapType;
		targetImage.header.imageType = sourceImage.header.imageType;
		targetImage.header.colorMapOrigin = sourceImage.header.colorMapOrigin;
		targetImage.header.colorMapLength = sourceImage.header.colorMapLength;
		targetImage.header.colorMapBitDepth = sourceImage.header.colorMapBitDepth;
		targetImage.header.originX = sourceImage.header.originX;
		targetImage.header.originY = sourceImage.header.originY;
		targetImage.header.width = targetWidth;
		targetImage.header.height = targetHeight;
		targetImage.header.pixelBitDepth = sourceImage.header.pixelBitDepth;
		targetImage.header.imageDescriptor = sourceImage.header.imageDescriptor;
		targetImage.header.imageId = sourceImage.header.imageId;
		targetImage.header.colorMap = sourceImage.header.colorMap;

		// Body
		targetImage.body.pixelByteDepth = sourceImage.header.pixelByteDepth();
		targetImage.body.rowStride = targetImage.header.width * targetImage.header.pixelByteDepth();

		const auto targetBufferSize{ targetImage.body.rowStride * targetImage.header.height };
		std::unique_ptr<uint8_t[]> targetBuffer(new uint8_t[targetBufferSize]);
		targetImage.body.pixels = targetBuffer.get();

		// Allocate a temporary buffer to hold our horizontal pass output.
		// We're using unique_ptr rather than vector because we want a fast and
		// smart way to allocate very large buffers without initialization.
		const auto tempBufferSize{ targetImage.body.rowStride * sourceImage.header.height };
		std::unique_ptr<uint8_t[]> tempBuffer(new uint8_t[tempBufferSize]);
		auto tempBufferPixels{ tempBuffer.get() };

		const auto sourceMappingWidth{ static_cast<float>(sourceImage.header.width - 1) };
		const auto targetMappingWidth{ static_cast<float>(targetImage.header.width - 1) };
		const auto mappingRatioX = sourceMappingWidth / targetMappingWidth;

		const auto sourceMappingHeight{ static_cast<float>(sourceImage.header.height - 1) };
		const auto targetMappingHeight{ static_cast<float>(targetImage.header.height - 1) };
		const auto mappingRatioY = sourceMappingHeight / targetMappingHeight;

		const auto sampler = KernelSampler::create(type);

		resampleDirection(sampler,
						  Horizontal,
						  mappingRatioX,
						  mappingRatioY,
						  sourceImage.header.width,
						  sourceImage.header.height,
						  sourceImage.body.pixels,
						  targetImage.header.width,
						  sourceImage.header.height,
						  tempBufferPixels);

		resampleDirection(sampler,
						  Vertical,
						  mappingRatioX,
						  mappingRatioY,
						  targetImage.header.width,
						  sourceImage.header.height,
						  tempBufferPixels,
						  targetImage.header.width,
						  targetImage.header.height,
						  targetImage.body.pixels);

		return true;
	}

	bool Resampler::resampleDirection(const std::shared_ptr<KernelSampler> sampler,
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

	bool Resampler::resamplePosition(const std::shared_ptr<KernelSampler> sampler,
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
