//#include <iostream>
#include <vector>

#include "stdio_file_interface.hpp"
#include "decoder.hpp"
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
	tga::Header header{};

	decoder.readHeader(header);

	tga::Image image{};
	image.pixelByteDepth = header.pixelByteDepth();
	image.rowStride = header.width * header.pixelByteDepth();

	const auto bufferSize{ image.rowStride * header.height };
	std::vector<uint8_t> buffer(bufferSize);
	image.pixels = buffer.data();

	decoder.readImage(header, image);

	std::fclose(f0);

	// Write target image file.
	auto targetFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };
	
	FILE* f1 = std::fopen(targetFilePath, "wb");
	tga::StdioFileInterface targetFile{ f1 };

	tga::Encoder encoder{ &sourceFile };
	encoder.writeHeader(header);
	encoder.writeImage(header, image);

	std::fclose(f1);

	return 0;
}
