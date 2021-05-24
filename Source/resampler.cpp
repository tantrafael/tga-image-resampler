#include "resampler.hpp"

namespace tga
{
	Resampler::Resampler()
	{}

	bool Resampler::resample(const Header& sourceHeader,
							 const Image& sourceImage,
							 Header& targetHeader,
							 Image& targetImage)
	{
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

		targetImage.pixelByteDepth = sourceImage.pixelByteDepth;
		//targetImage.rowStride = targetHeader.width * targetImage.pixelByteDepth;

		const unsigned int bufferSize{ targetImage.rowStride * sourceHeader.height };
		std::unique_ptr<uint8_t[]> buffer(new uint8_t[bufferSize]);

		float_t h_ratio = static_cast<float_t>(sourceHeader.width - 1)
						/ static_cast<float_t>(targetHeader.width - 1);

		float_t v_ratio = static_cast<float_t>(sourceHeader.height - 1)
						/ static_cast<float_t>(targetHeader.height - 1);

		// Horizontal resampling.
		for (uint32_t j = 0; j < sourceHeader.height; ++j)
		{
			for (uint32_t i = 0; i < targetHeader.width; ++i)
			{
				uint8_t* output = BLOCK_OFFSET_RGB24(buffer.get(), targetHeader.width, i, j);

				// Determine the sub-pixel location of our *target* (i,j)
				// coordinate, in the space of our source image.
				float_t f_x = (float_t) i * h_ratio;
				float_t f_y = (float_t) j;

				sampleKernelBicubicH(sourceImage.pixels,
									 sourceHeader.width,
									 sourceHeader.height,
									 f_x,
									 f_y,
									 output);
			}
		}

		// Vertical resampling.
		for (uint32_t j = 0; j < targetHeader.height; ++j)
		{
			for (uint32_t i = 0; i < targetHeader.width; i++)
			{
				uint8_t* output = BLOCK_OFFSET_RGB24(targetImage.pixels, targetHeader.width, i, j);

				// Determine the sub-pixel location of our *target* (i,j)
				// coordinate, in the space of our temp image.
				float_t f_x = (float_t) i;
				float_t f_y = (float_t) j * v_ratio;

				sampleKernelBicubicV(buffer.get(),
									 targetHeader.width,
									 sourceHeader.height,
									 f_x,
									 f_y,
									 output);
			}
		}

		return true;
	}

	bool Resampler::sampleKernelBicubicH(uint8_t* sourcePixels,
										 uint32_t sourceWidth,
										 uint32_t sourceHeight,
										 float_t f_x,
										 float_t f_y,
										 uint8_t* output)
	{
		float_t sample_count = 0;
		float_t total_samples[3] = {0};

		for (int32_t i = -2; i < 2; ++i)
		{
			int32_t i_x = (int32_t)f_x + i;
			int32_t i_y = (int32_t)f_y;

			if (i_x < 0 || i_y < 0 || i_x > sourceWidth - 1 || i_y > sourceHeight - 1)
			{
				continue;
			}

			float_t x_delta = (float_t)f_x - i_x;
			float_t distance = fabs(x_delta);
			float_t weight = bicubicWeight(0.0, 1.0, distance);
			uint8_t* src_pixel = BLOCK_OFFSET_RGB24(sourcePixels, sourceWidth, i_x, i_y);

			/* accumulate bicubic weighted samples from the source. */
			total_samples[0] += src_pixel[0] * weight;
			total_samples[1] += src_pixel[1] * weight;
			total_samples[2] += src_pixel[2] * weight;

			/* record the total weights of the sample for later normalization. */
			sample_count += weight;
		}

		/* Normalize our bicubic sum back to the valid pixel range. */
		float_t scale_factor = 1.0f / sample_count;
		output[0] = clipRange(scale_factor * total_samples[0], 0, 255);
		output[1] = clipRange(scale_factor * total_samples[1], 0, 255);
		output[2] = clipRange(scale_factor * total_samples[2], 0, 255);

		return true;
	}

	bool Resampler::sampleKernelBicubicV(uint8_t* sourcePixels,
										 uint32_t sourceWidth,
										 uint32_t sourceHeight,
										 float_t f_x,
										 float_t f_y,
										 uint8_t* output)
	{
		float_t sample_count = 0;
		float_t total_samples[3] = {0};

		for (int32_t i = -2; i < 2; ++i)
		{
			int32_t i_x = (int32_t)f_x;
			int32_t i_y = (int32_t)f_y + i;

			if (i_x < 0 || i_y < 0 || i_x > sourceWidth - 1 || i_y > sourceHeight - 1)
			{
				continue;
			}

			float_t y_delta = (float_t)f_y - i_y;
			float_t distance = fabs(y_delta);
			float_t weight = bicubicWeight(0.0, 1.0, distance);
			uint8_t* src_pixel = BLOCK_OFFSET_RGB24(sourcePixels, sourceWidth, i_x, i_y);

			/* accumulate bicubic weighted samples from the source. */
			total_samples[0] += src_pixel[0] * weight;
			total_samples[1] += src_pixel[1] * weight;
			total_samples[2] += src_pixel[2] * weight;

			/* record the total weights of the sample for later normalization. */
			sample_count += weight;
		}

		/* Normalize our bicubic sum back to the valid pixel range. */
		float_t scale_factor = 1.0f / sample_count;
		output[0] = clipRange(scale_factor * total_samples[0], 0, 255);
		output[1] = clipRange(scale_factor * total_samples[1], 0, 255);
		output[2] = clipRange(scale_factor * total_samples[2], 0, 255);

		return true;
	}
}
