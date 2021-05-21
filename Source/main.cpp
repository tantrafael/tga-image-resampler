//#include <iostream>
#include <vector>

#include "stdio_file_interface.hpp"
#include "decoder.hpp"
#include "resampler.hpp"
#include "encoder.hpp"
#include "header.hpp"
#include "image.hpp"

int main(int argc, const char * argv[]) {
	// "sample_640×426.tga"
	// "shuttle.tga"
	// "football_seal.tga"

	// Read source image file.
	auto sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_640×426.tga" };
	FILE* f0 = std::fopen(sourceFilePath, "rb");
	tga::StdioFileInterface sourceFile{ f0 };

	tga::Decoder decoder{ &sourceFile };
	tga::Header sourceHeader{};
	decoder.readHeader(sourceHeader);

	tga::Image sourceImage{};
	sourceImage.pixelByteDepth = sourceHeader.pixelByteDepth();
	sourceImage.rowStride = sourceHeader.width * sourceHeader.pixelByteDepth();
	//const auto bufferSize{ sourceImage.rowStride * sourceHeader.height };
	const unsigned int bufferSize{ sourceImage.rowStride * sourceHeader.height };
	std::vector<uint8_t> sourceBuffer(bufferSize);
	sourceImage.pixels = sourceBuffer.data();
	decoder.readImage(sourceHeader, sourceImage);

	std::fclose(f0);

	// Resample image.
	tga::Header targetHeader{};
	tga::Image targetImage{};
	tga::Resampler resampler{};
	std::vector<uint8_t> targetBuffer(bufferSize);
	targetImage.pixels = targetBuffer.data();
	resampler.resample(sourceHeader, sourceImage, targetHeader, targetImage, bufferSize);

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
