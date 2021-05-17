//#include <iostream>
#include <vector>

#include "decoder.hpp"
#include "header.hpp"
#include "image.hpp"

int main(int argc, const char * argv[]) {
	std::string sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_640×426.tga" };
	//std::string sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/shuttle.tga" };
	//std::string sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/football_seal.tga" };
	//std::string sourceFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };

	//std::ifstream sourceImageFile{ sourceImageFilePath, std::ifstream::binary|std::ifstream::ate };
	std::ifstream sourceFile{ sourceFilePath, std::ifstream::binary };

	if (!sourceFile)
	{
		std::cerr << "Source file could not be opened for reading.\n";
		return 1;
	}

	//tga::Decoder decoder{ sourceImageFile };
	tga::Decoder decoder{ &sourceFile };
	tga::Header header{};

	decoder.readHeader(header);

	tga::Image image;
	image.pixelByteDepth = header.pixelByteDepth();
	image.rowStride = header.width * header.pixelByteDepth();

	std::vector<uint8_t> buffer(image.rowStride * header.height);
	//image.pixels = &buffer[0];
	image.pixels = buffer.data();

	decoder.readImage(header, image);

	/*
	// Write target image file.
	std::ofstream targetImageFile{ targetImageFilePath, std::ifstream::binary };

	if (!targetImageFile)
	{
		std::cerr << "Target image file could not be opened for writing.\n";
		return 1;
	}

	targetImageFile.write(sourceImageBuffer.data(), sourceImageFileSize);

	std::cout << "Target image file written.\n";
	*/

	return 0;
}
