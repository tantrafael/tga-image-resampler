#include "resampler.hpp"

#include "resampler_helper.hpp"

namespace tga
{
	Resampler::Resampler()
	{}

	bool Resampler::resample(const Image& sourceImage,
							 const int destinationWidth,
							 const int destinationHeight,
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

		auto headerResamplingResult
		{
			resampleHeader(sourceImage,
						   destinationWidth,
						   destinationHeight,
						   destinationImage)
		};

		auto bodyResamplingResult
		{
			resampleBody(sourceImage, kernel, destinationImage)
		};

		return (headerResamplingResult && bodyResamplingResult);
	}

	bool Resampler::resampleHeader(const Image& sourceImage,
								   const int destinationWidth,
								   const int destinationHeight,
								   Image& destinationImage)
	{
		const bool isValidInput = (destinationWidth > 0
								   && destinationHeight > 0);

		if (!isValidInput)
		{
			return false;
		}

		destinationImage.idLength = sourceImage.idLength;
		destinationImage.colorMapType = sourceImage.colorMapType;
		destinationImage.imageType = sourceImage.imageType;
		destinationImage.colorMapOrigin = sourceImage.colorMapOrigin;
		destinationImage.colorMapLength = sourceImage.colorMapLength;
		destinationImage.colorMapBitDepth = sourceImage.colorMapBitDepth;
		destinationImage.originX = sourceImage.originX;
		destinationImage.originY = sourceImage.originY;
		destinationImage.width = destinationWidth;
		destinationImage.height = destinationHeight;
		destinationImage.pixelBitDepth = sourceImage.pixelBitDepth;
		destinationImage.imageDescriptor = sourceImage.imageDescriptor;
		destinationImage.imageId = sourceImage.imageId;
		destinationImage.colorMap = sourceImage.colorMap;

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

		destinationImage.pixelByteDepth = sourceImage.pixelByteDepth;
		destinationImage.rowStride = destinationImage.width * destinationImage.pixelByteDepth;

		const auto destinationBufferSize{ destinationImage.rowStride * destinationImage.height };
		std::unique_ptr<uint8_t[]> destinationBuffer(new uint8_t[destinationBufferSize]);
		destinationImage.pixels = destinationBuffer.get();

		// Allocate a temporary buffer to hold our horizontal pass output.
		// We're using unique_ptr rather than vector because we want a fast and
		// smart way to allocate very large buffers without initialization.
		const auto tempBufferSize{ destinationImage.rowStride * sourceImage.height };
		std::unique_ptr<uint8_t[]> tempBuffer(new uint8_t[tempBufferSize]);
		auto tempBufferPixels{ tempBuffer.get() };

		const auto sourceMappingWidth{ static_cast<float>(sourceImage.width - 1) };
		const auto destinationMappingWidth{ static_cast<float>(destinationImage.width - 1) };
		const auto mappingRatioX = sourceMappingWidth / destinationMappingWidth;

		const auto sourceMappingHeight{ static_cast<float>(sourceImage.height - 1) };
		const auto destinationMappingHeight{ static_cast<float>(destinationImage.height - 1) };
		const auto mappingRatioY = sourceMappingHeight / destinationMappingHeight;

		const auto sampler = KernelSampler::create(kernel);

		// Horizontal pass, from source image to temporary buffer.
		auto horizontalResamplingResult
		{
			resampleDirection(sampler,
							  Horizontal,
							  mappingRatioX,
							  mappingRatioY,
							  sourceImage.width,
							  sourceImage.height,
							  sourceImage.pixels,
							  destinationImage.width,
							  sourceImage.height,
							  tempBufferPixels)
		};

		// Vertical pass, from temporary buffer to destination image.
		auto verticalResamplingResult
		{
			resampleDirection(sampler,
							  Vertical,
							  mappingRatioX,
							  mappingRatioY,
							  destinationImage.width,
							  sourceImage.height,
							  tempBufferPixels,
							  destinationImage.width,
							  destinationImage.height,
							  destinationImage.pixels)
		};

		return (horizontalResamplingResult && verticalResamplingResult);
	}

	bool Resampler::resampleDirection(const std::shared_ptr<KernelSampler> sampler,
									  const ResamplingDirection direction,
									  const float mappingRatioX,
									  const float mappingRatioY,
									  const int inputWidth,
									  const int inputHeight,
									  uint8_t* const inputPixels,
									  const int outputWidth,
									  const int outputHeight,
									  uint8_t* const outputPixels)
	{
		const bool isValidInput = (sampler != nullptr
								   && mappingRatioX > 0
								   && mappingRatioY > 0
								   && inputWidth > 0
								   && inputHeight > 0
								   && inputPixels != nullptr
								   && outputWidth > 0
								   && outputHeight > 0
								   && outputPixels != nullptr);

		if (!isValidInput)
		{
			return false;
		}

		for (int outputRow = 0; outputRow < outputHeight; ++outputRow)
		{
			for (int outputCol = 0; outputCol < outputWidth; ++outputCol)
			{
				if (!resamplePosition(sampler,
									  direction,
									  mappingRatioX,
									  mappingRatioY,
									  outputRow,
									  outputCol,
									  inputPixels,
									  inputWidth,
									  inputHeight,
									  outputWidth,
									  outputPixels))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool Resampler::resamplePosition(const std::shared_ptr<KernelSampler> sampler,
									 const ResamplingDirection direction,
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
		const bool isValidInput = (sampler != nullptr
								   && mappingRatioX > 0
								   && mappingRatioY > 0
								   && outputRow >= 0
								   && outputCol >= 0
								   && inputPixels != nullptr
								   && inputWidth > 0
								   && inputHeight > 0
								   && outputWidth > 0
								   && outputPixels != nullptr);

		if (!isValidInput)
		{
			return false;
		}

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

		auto samplingResult
		{
			sampler->sample(subPixelPosX,
							subPixelPosY,
							direction,
							inputPixels,
							inputWidth,
							inputHeight,
							sampleCount,
							totalSamples)
		};

		if (samplingResult)
		{
			// Normalize our sum back to the valid pixel range.
			const float scaleFactor = 1.0f / sampleCount;

			uint8_t* output = BLOCK_OFFSET_RGB32(outputPixels,
												 outputWidth,
												 outputCol,
												 outputRow);

			output[0] = clipRange(scaleFactor * totalSamples[0], 0, 255);
			output[1] = clipRange(scaleFactor * totalSamples[1], 0, 255);
			output[2] = clipRange(scaleFactor * totalSamples[2], 0, 255);
		}

		return samplingResult;
	}
}
