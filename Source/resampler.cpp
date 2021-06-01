#include "resampler.hpp"
#include <cmath>
#include <unordered_map>

namespace tga
{
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

		const auto sourceMappingWidth{ static_cast<float>(sourceHeader.width - 1) };
		const auto targetMappingWidth{ static_cast<float>(targetHeader.width - 1) };
		const auto mappingRatioX = sourceMappingWidth / targetMappingWidth;

		const auto sourceMappingHeight{ static_cast<float>(sourceHeader.height - 1) };
		const auto targetMappingHeight{ static_cast<float>(targetHeader.height - 1) };
		const auto mappingRatioY = sourceMappingHeight / targetMappingHeight;

		// Horizontal resampling.
		for (int row = 0; row < sourceHeader.height; ++row)
		{
			for (int col = 0; col < targetHeader.width; ++col)
			{
				uint8_t* output = BLOCK_OFFSET_RGB32(buffer.get(),
													 targetHeader.width,
													 col,
													 row);

				// Determine the sub-pixel location of our target (col, row)
				// coordinate, in the space of our source image.
				auto subPixelPosX = static_cast<float>(col * mappingRatioX);
				auto subPixelPosY = static_cast<float>(row);

				/*
				sampleKernel(sourceImage.pixels,
							 sourceHeader.width,
							 sourceHeader.height,
							 Horizontal,
							 subPixelPosX,
							 subPixelPosY,
							 type,
							 mappingRatioX,
							 mappingRatioY,
							 output);
				*/

				//bar(subPixelPosX, subPixelPosY, output);

				foo(Horizontal,
					sourceImage.pixels,
					sourceHeader.width,
					sourceHeader.height,
					subPixelPosX,
					subPixelPosY,
					mappingRatioX,
					mappingRatioY,
					output);
			}
		}

		// Vertical resampling.
		for (int row = 0; row < targetHeader.height; ++row)
		{
			for (int col = 0; col < targetHeader.width; col++)
			{
				uint8_t* output = BLOCK_OFFSET_RGB32(targetImage.pixels,
													 targetHeader.width,
													 col,
													 row);

				// Determine the sub-pixel location of our target (col, row)
				// coordinate, in the space of our source image.
				auto subPixelPosX = static_cast<float>(col);
				auto subPixelPosY = static_cast<float>(row * mappingRatioY);

				/*
				sampleKernel(buffer.get(),
							 targetHeader.width,
							 sourceHeader.height,
							 Vertical,
							 subPixelPosX,
							 subPixelPosY,
							 type,
							 mappingRatioX,
							 mappingRatioY,
							 output);
				*/

				foo(Vertical,
					buffer.get(),
					targetHeader.width,
					sourceHeader.height,
					subPixelPosX,
					subPixelPosY,
					mappingRatioX,
					mappingRatioY,
					output);
			}
		}

		return true;
	}

	bool Resampler::foo(KernelDirection direction,
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
		float coeffB{ 0.0f };
		float coeffC{ 1.0f };

		for (int i = -2; i < 2; ++i)
		{
			float distance{};
			uint8_t* sourcePixel{};

			if (!getSourcePixel(subPixelPosX,
								subPixelPosY,
								direction,
								i,
								pixels,
								width,
								height,
								distance,
								sourcePixel))
			{
				continue;
			}

			const auto weight{ bicubicWeight(coeffB, coeffC, distance) };

			accumulateSamples(sourcePixel, weight, totalSamples, sampleCount);
		}

		// Normalize our bicubic sum back to the valid pixel range.
		float scaleFactor = 1.0f / sampleCount;
		output[0] = clipRange(scaleFactor * totalSamples[0], 0, 255);
		output[1] = clipRange(scaleFactor * totalSamples[1], 0, 255);
		output[2] = clipRange(scaleFactor * totalSamples[2], 0, 255);

		return true;
	}

	bool Resampler::getSourcePixel(const float subPixelPosX,
								   const float subPixelPosY,
								   const KernelDirection direction,
								   const int i,
								   uint8_t* pixels,
								   const int32_t width,
								   const int32_t height,
								   float& distance,
								   uint8_t*& sourcePixel)
	{
		int32_t samplePosX{};
		int32_t samplePosY{};
		float delta{};

		if (direction == Horizontal)
		{
			samplePosX = static_cast<int32_t>(subPixelPosX + i);
			samplePosY = static_cast<int32_t>(subPixelPosY);
			delta = static_cast<float>(subPixelPosX - samplePosX);
		}
		else if (direction == Vertical)
		{
			samplePosX = static_cast<int32_t>(subPixelPosX);
			samplePosY = static_cast<int32_t>(subPixelPosY + i);
			delta = static_cast<float>(subPixelPosY - samplePosY);
		}

		const bool isValidSamplePos = (samplePosX >= 0
									   && samplePosY >= 0
									   && samplePosX < width
									   && samplePosY < height);

		if (!isValidSamplePos)
		{
			return false;
		}

		distance = fabs(delta);
		sourcePixel = BLOCK_OFFSET_RGB32(pixels, width, samplePosX, samplePosY);

		return true;
	}

	void Resampler::accumulateSamples(const uint8_t* sourcePixel,
									  const float weight,
									  float (&totalSamples)[3],
									  float& sampleCount)
	{
		// Accumulate weighted samples from the source.
		totalSamples[0] += sourcePixel[0] * weight;
		totalSamples[1] += sourcePixel[1] * weight;
		totalSamples[2] += sourcePixel[2] * weight;

		// Record the total weights of the sample for later normalization.
		sampleCount += weight;
	}

	/*
	bool Resampler::sampleKernel(uint8_t* pixels,
								 uint32_t width,
								 uint32_t height,
								 KernelDirection direction,
								 float subPixelPosX,
								 float subPixelPosY,
								 KernelType type,
								 float mappingRatioX,
								 float mappingRatioY,
								 uint8_t* output)
	{
		switch (type)
		{
			case Bicubic:
				return sampleKernelBicubic(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   0.0f,
										   1.0f,
										   output);
			case Catmull:
				return sampleKernelBicubic(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   0.0f,
										   0.5f,
										   output);
			case Mitchell:
				return sampleKernelBicubic(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   1.0f / 3.0f,
										   1.0f / 3.0f,
										   output);
			case Cardinal:
				return sampleKernelBicubic(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   0.0f,
										   0.75f,
										   output);
			case BSpline:
				return sampleKernelBicubic(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   1.0f,
										   0.0f,
										   output);

			case Lanczos:
				return sampleKernelLanczos(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   1,
										   output);

			case Lanczos2:
				return sampleKernelLanczos(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   2,
										   output);

			case Lanczos3:
				return sampleKernelLanczos(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   3,
										   output);

			case Lanczos4:
				return sampleKernelLanczos(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   4,
										   output);

			case Lanczos5:
				return sampleKernelLanczos(pixels,
										   width,
										   height,
										   direction,
										   subPixelPosX,
										   subPixelPosY,
										   5,
										   output);
		}

		return false;
	}
	*/

	/*
	bool Resampler::sampleKernel(uint8_t* pixels,
								 uint32_t width,
								 uint32_t height,
								 KernelDirection direction,
								 float subPixelPosX,
								 float subPixelPosY,
								 KernelType type,
								 float mappingRatioX,
								 float mappingRatioY,
								 uint8_t* output)
	{
		std::unordered_map<KernelType, float> u =
		{
			{Bicubic, 0.0f},
			{Catmull, 0.0f},
			{Mitchell, 0.0f},
			{Cardinal, 0.0f},
			{BSpline, 0.0f}
		};

		if (type == Bicubic || type == Catmull || type == Mitchell || type == Cardinal || type == BSpline)
		{
			float coeffB{ u[Bicubic] };
			float coeffC{ 1.0f };

			sampleKernelBicubic(pixels,
								width,
								height,
								direction,
								subPixelPosX,
								subPixelPosY,
								coeffB,
								coeffC,
								output);
		}

		return true;
	}
	*/

	/*
	bool Resampler::sampleKernelBicubic(uint8_t* pixels,
										uint32_t width,
										uint32_t height,
										KernelDirection direction,
										float subPixelPosX,
										float subPixelPosY,
										float coeffB,
										float coeffC,
										uint8_t* output)
	{
		const bool isValidInput = (pixels != nullptr
								   && width >= 0
								   && height >= 0
								   && subPixelPosX >= 0.0f
								   && subPixelPosY >= 0.0f
								   && coeffB >= 0.0f
								   && coeffC >= 0.0f
								   && output != nullptr);

		if (!isValidInput)
		{
			return false;
		}

		float sampleCount = 0;
		float totalSamples[3] = {0};

		for (int i = -2; i < 2; ++i)
		{
			int32_t samplePosX{};
			int32_t samplePosY{};
			float delta{};

			if (direction == Horizontal)
			{
				samplePosX = static_cast<int32_t>(subPixelPosX + i);
				samplePosY = static_cast<int32_t>(subPixelPosY);
				delta = static_cast<float>(subPixelPosX - samplePosX);
			}
			else if (direction == Vertical)
			{
				samplePosX = static_cast<int32_t>(subPixelPosX);
				samplePosY = static_cast<int32_t>(subPixelPosY + i);
				delta = static_cast<float>(subPixelPosY - samplePosY);
			}

			if (samplePosX < 0
				|| samplePosY < 0
				|| samplePosX > width - 1
				|| samplePosY > height - 1)
			{
				continue;
			}

			const auto distance{ fabs(delta) };
			const auto weight{ bicubicWeight(coeffB, coeffC, distance) };
			const auto sourcePixel = BLOCK_OFFSET_RGB32(pixels, width, samplePosX, samplePosY);

			// Accumulate bicubic weighted samples from the source.
			totalSamples[0] += sourcePixel[0] * weight;
			totalSamples[1] += sourcePixel[1] * weight;
			totalSamples[2] += sourcePixel[2] * weight;

			// Record the total weights of the sample for later normalization.
			sampleCount += weight;
		}

		// Normalize our bicubic sum back to the valid pixel range.
		float scaleFactor = 1.0f / sampleCount;
		output[0] = clipRange(scaleFactor * totalSamples[0], 0, 255);
		output[1] = clipRange(scaleFactor * totalSamples[1], 0, 255);
		output[2] = clipRange(scaleFactor * totalSamples[2], 0, 255);

		return true;
	}

	bool Resampler::sampleKernelLanczos(uint8_t* pixels,
										uint32_t width,
										uint32_t height,
										KernelDirection direction,
										float subPixelPosX,
										float subPixelPosY,
										float coeffA,
										uint8_t* output)
	{
		const bool isValidInput = (pixels != nullptr
								   && width >= 0
								   && height >= 0
								   && subPixelPosX >= 0.0f
								   && subPixelPosY >= 0.0f
								   && coeffA >= 0.0f
								   && output != nullptr);

		if (!isValidInput)
		{
			return false;
		}

		int32_t radius = coeffA;
		float sampleCount = 0;
		float totalSamples[3] = {0};

		for (int i = -radius; i < radius; ++i)
		{
			int32_t samplePosX{};
			int32_t samplePosY{};
			float delta{};

			if (direction == Horizontal)
			{
				samplePosX = static_cast<int32_t>(subPixelPosX + i);
				samplePosY = static_cast<int32_t>(subPixelPosY);
				delta = static_cast<float>(subPixelPosX - samplePosX);
			}
			else if (direction == Vertical)
			{
				samplePosX = static_cast<int32_t>(subPixelPosX);
				samplePosY = static_cast<int32_t>(subPixelPosY + i);
				delta = static_cast<float>(subPixelPosY - samplePosY);
			}

			if (samplePosX < 0
				|| samplePosY < 0
				|| samplePosX > width - 1
				|| samplePosY > height - 1)
			{
				continue;
			}

			const auto distance{ fabs(delta) };
			const auto weight{ lanczosWeight(coeffA, distance) };
			const auto sourcePixel = BLOCK_OFFSET_RGB32(pixels, width, samplePosX, samplePosY);

			// Accumulate bicubic weighted samples from the source.
			totalSamples[0] += sourcePixel[0] * weight;
			totalSamples[1] += sourcePixel[1] * weight;
			totalSamples[2] += sourcePixel[2] * weight;

			// Record the total weights of the sample for later normalization.
			sampleCount += weight;
		}

		// Normalize our bicubic sum back to the valid pixel range.
		float scaleFactor = 1.0f / sampleCount;
		output[0] = clipRange(scaleFactor * totalSamples[0], 0, 255);
		output[1] = clipRange(scaleFactor * totalSamples[1], 0, 255);
		output[2] = clipRange(scaleFactor * totalSamples[2], 0, 255);

		return true;
	}
	*/
}
