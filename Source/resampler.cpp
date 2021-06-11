#include "resampler.hpp"

#include "resampler_helper.hpp"

namespace tga
{
	Resampler::Resampler()
	{}

	bool Resampler::resample(const Image& sourceImage,
							 const unsigned int destinationWidth,
							 const unsigned int destinationHeight,
							 const KernelType kernel,
							 Image& destinationImage)
	{
		const bool isValidInput = (destinationWidth > 0
								   && destinationHeight > 0
								   && kernel != Unknown);

		if (!isValidInput)
		{
			return false;
		}

		resampleHeader(sourceImage, destinationWidth, destinationHeight, destinationImage);
		resampleBody(sourceImage, kernel, destinationImage);

		return true;
	}

	bool Resampler::resampleHeader(const Image& sourceImage,
								   const unsigned int destinationWidth,
								   const unsigned int destinationHeight,
								   Image& destinationImage)
	{
		const bool isValidInput = (destinationWidth > 0
								   && destinationHeight > 0);

		if (!isValidInput)
		{
			return false;
		}

		const auto& sourceHeader{ sourceImage.header };
		auto& destinationHeader{ destinationImage.header };

		destinationHeader.idLength = sourceHeader.idLength;
		destinationHeader.colorMapType = sourceHeader.colorMapType;
		destinationHeader.imageType = sourceHeader.imageType;
		destinationHeader.colorMapOrigin = sourceHeader.colorMapOrigin;
		destinationHeader.colorMapLength = sourceHeader.colorMapLength;
		destinationHeader.colorMapBitDepth = sourceHeader.colorMapBitDepth;
		destinationHeader.originX = sourceHeader.originX;
		destinationHeader.originY = sourceHeader.originY;
		destinationHeader.width = destinationWidth;
		destinationHeader.height = destinationHeight;
		destinationHeader.pixelBitDepth = sourceHeader.pixelBitDepth;
		destinationHeader.imageDescriptor = sourceHeader.imageDescriptor;
		destinationHeader.imageId = sourceHeader.imageId;
		destinationHeader.colorMap = sourceHeader.colorMap;

		return true;
	}

	bool Resampler::resampleBody(const Image& sourceImage,
								 const KernelType kernel,
								 Image& destinationImage)
	{
		const bool isValidInput = (kernel != Unknown);

		if (!isValidInput)
		{
			return false;
		}

		const auto& sourceHeader{ sourceImage.header };
		const auto& sourceBody{ sourceImage.body };
		const auto& destinationHeader{ destinationImage.header };
		auto& destinationBody{ destinationImage.body };

		destinationBody.pixelByteDepth = sourceHeader.pixelByteDepth();
		destinationBody.rowStride = destinationHeader.width * destinationHeader.pixelByteDepth();

		const auto destinationBufferSize{ destinationBody.rowStride * destinationHeader.height };
		std::unique_ptr<uint8_t[]> destinationBuffer(new uint8_t[destinationBufferSize]);
		destinationBody.pixels = destinationBuffer.get();

		// Allocate a temporary buffer to hold our horizontal pass output.
		// We're using unique_ptr rather than vector because we want a fast and
		// smart way to allocate very large buffers without initialization.
		const auto tempBufferSize{ destinationBody.rowStride * sourceHeader.height };
		std::unique_ptr<uint8_t[]> tempBuffer(new uint8_t[tempBufferSize]);
		auto tempBufferPixels{ tempBuffer.get() };

		const auto sourceMappingWidth{ static_cast<float>(sourceHeader.width - 1) };
		const auto destinationMappingWidth{ static_cast<float>(destinationHeader.width - 1) };
		const auto mappingRatioX = sourceMappingWidth / destinationMappingWidth;

		const auto sourceMappingHeight{ static_cast<float>(sourceHeader.height - 1) };
		const auto destinationMappingHeight{ static_cast<float>(destinationHeader.height - 1) };
		const auto mappingRatioY = sourceMappingHeight / destinationMappingHeight;

		const auto sampler = KernelSampler::create(kernel);

		// Horizontal pass, from source image to temporary buffer.
		resampleDirection(sampler,
						  Horizontal,
						  mappingRatioX,
						  mappingRatioY,
						  sourceHeader.width,
						  sourceHeader.height,
						  sourceBody.pixels,
						  destinationHeader.width,
						  sourceHeader.height,
						  tempBufferPixels);

		// Vertical pass, from temporary buffer to destination image.
		resampleDirection(sampler,
						  Vertical,
						  mappingRatioX,
						  mappingRatioY,
						  destinationHeader.width,
						  sourceHeader.height,
						  tempBufferPixels,
						  destinationHeader.width,
						  destinationHeader.height,
						  destinationBody.pixels);

		return true;
	}

	bool Resampler::resampleDirection(const std::shared_ptr<KernelSampler> sampler,
									  const KernelDirection direction,
									  const float mappingRatioX,
									  const float mappingRatioY,
									  const unsigned int inputWidth,
									  const unsigned int inputHeight,
									  uint8_t* const inputPixels,
									  const unsigned int outputWidth,
									  const unsigned int outputHeight,
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
									 const unsigned int outputRow,
									 const unsigned int outputCol,
									 uint8_t* const inputPixels,
									 const unsigned int inputWidth,
									 const unsigned int inputHeight,
									 const unsigned int outputWidth,
									 uint8_t* const outputPixels)
	{
		// Determine the sub-pixel location of our destination (col, row)
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

		uint8_t* output = BLOCK_OFFSET_RGB32(outputPixels,
											 outputWidth,
											 outputCol,
											 outputRow);

		output[0] = clipRange(scaleFactor * totalSamples[0], 0, 255);
		output[1] = clipRange(scaleFactor * totalSamples[1], 0, 255);
		output[2] = clipRange(scaleFactor * totalSamples[2], 0, 255);

		return true;
	}
}
