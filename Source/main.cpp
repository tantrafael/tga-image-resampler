//#include <iostream>
#include <vector>

#include "stdio_file_interface.hpp"
#include "decoder.hpp"
#include "resampler.hpp"
#include "encoder.hpp"
#include "header.hpp"
#include "image.hpp"

int main(int argc, const char * argv[]) {
	// Read source image file.
	auto sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_640×426.tga" };
	FILE* f0 = std::fopen(sourceFilePath, "rb");
	tga::StdioFileInterface sourceFile{ f0 };

	tga::Header sourceHeader{};
	tga::Image sourceImage{};
	tga::Decoder decoder{ &sourceFile };

	decoder.readHeader(sourceHeader);
	sourceImage.pixelByteDepth = sourceHeader.pixelByteDepth();
	sourceImage.rowStride = sourceHeader.width * sourceHeader.pixelByteDepth();
	const unsigned int sourceBufferSize{ sourceImage.rowStride * sourceHeader.height };
	std::vector<uint8_t> sourceBuffer(sourceBufferSize);
	sourceImage.pixels = sourceBuffer.data();
	decoder.readImage(sourceHeader, sourceImage);

	std::fclose(f0);

	// Resample image.
	tga::Header targetHeader{};
	tga::Image targetImage{};
	tga::Resampler resampler{};

	targetHeader.width = 320;
	targetHeader.height = 213;
	targetImage.pixelByteDepth = sourceImage.pixelByteDepth;
	targetImage.rowStride = targetHeader.width * targetImage.pixelByteDepth;
	const unsigned int targetBufferSize{ targetImage.rowStride * targetHeader.height };
	std::vector<uint8_t> targetBuffer(targetBufferSize);
	targetImage.pixels = targetBuffer.data();
	//resampler.resample(sourceHeader, sourceImage, targetHeader, targetImage, tga::Bicubic);
	resampler.resample(sourceHeader, sourceImage, targetHeader, targetImage, tga::Lanczos5);

	// Write target image file.
	auto targetFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };
	FILE* f1 = std::fopen(targetFilePath, "wb");
	tga::StdioFileInterface targetFile{ f1 };

	tga::Encoder encoder{ &targetFile };
	encoder.writeHeader(targetHeader);
	encoder.writeImage(targetHeader, targetImage);

	std::fclose(f1);

	return 0;
}
