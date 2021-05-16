//#include <iostream>

#include "decoder.hpp"
#include "header.hpp"

int main(int argc, const char * argv[]) {
	//std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_640×426.tga" };
	//std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/shuttle.tga" };
	std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/football_seal.tga" };
	//std::string targetImageFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };

	//std::ifstream sourceImageFile{ sourceImageFilePath, std::ifstream::binary|std::ifstream::ate };
	std::ifstream sourceImageFile{ sourceImageFilePath, std::ifstream::binary|std::ifstream::ate };

	if (!sourceImageFile)
	{
		std::cerr << "Source image file could not be opened for reading.\n";
		return 1;
	}

	//tga::Decoder decoder(sourceImageFile);
	//tga::Decoder decoder{ sourceImageFile };
	tga::Decoder decoder{ &sourceImageFile };
	tga::Header header{};

	decoder.readHeader(header);

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
