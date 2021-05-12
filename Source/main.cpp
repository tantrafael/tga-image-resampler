#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, const char * argv[]) {
	//std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_640×426.tga" };
	//std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/shuttle.tga" };
	std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/football_seal.tga" };
	std::string targetImageFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };

	// Read source image file into memory.
	std::ifstream sourceImageFile{ sourceImageFilePath, std::ifstream::binary|std::ifstream::ate };

	if (!sourceImageFile)
	{
		std::cerr << "Source image file could not be opened for reading.\n";
		return 1;
	}
 
	auto sourceImageFileSize = sourceImageFile.tellg();
	std::vector<char> sourceImageBuffer(sourceImageFileSize);

	sourceImageFile.seekg(0);
	sourceImageFile.read(sourceImageBuffer.data(), sourceImageFileSize);

	std::cout << "Source image file read.\n";

	sourceImageFile.seekg(0);

	uint8_t idLength{};
	sourceImageFile.read((char *) &idLength, sizeof(uint8_t));
	std::cout << "ID length: " << (int) idLength << '\n';

	uint8_t colorMapType{};
	sourceImageFile.read((char *) &colorMapType, sizeof(uint8_t));
	std::cout << "Color map type: " << (int) colorMapType << '\n';

	uint8_t imageType{};
	sourceImageFile.read((char *) &imageType, sizeof(uint8_t));
	std::cout << "Image type: " << (int) imageType << '\n';

	uint16_t colorMapOrigin{};
	sourceImageFile.read((char *) &colorMapOrigin, sizeof(uint16_t));
	std::cout << "Color map first entry index: " << (int) colorMapOrigin << '\n';

	uint16_t colorMapLength{};
	sourceImageFile.read((char *) &colorMapLength, sizeof(uint16_t));
	std::cout << "Color map length: " << (int) colorMapLength << '\n';

	uint8_t colorMapDepth{};
	sourceImageFile.read((char *) &colorMapDepth, sizeof(uint8_t));
	std::cout << "Color map entry size: " << (int) colorMapDepth << '\n';

	uint16_t xOrigin{};
	sourceImageFile.read((char *) &xOrigin, sizeof(uint16_t));
	std::cout << "X-origin: " << (int) xOrigin << '\n';

	uint16_t yOrigin{};
	sourceImageFile.read((char *) &yOrigin, sizeof(uint16_t));
	std::cout << "Y-origin: " << (int) yOrigin << '\n';

	uint16_t width{};
	sourceImageFile.read((char *) &width, sizeof(uint16_t));
	std::cout << "Image width: " << (int) width << '\n';

	uint16_t height{};
	sourceImageFile.read((char *) &height, sizeof(uint16_t));
	std::cout << "Image height: " << (int) height << '\n';

	uint8_t pixelDepth{};
	sourceImageFile.read((char *) &pixelDepth, sizeof(uint8_t));
	std::cout << "Pixel depth: " << (int) pixelDepth << '\n';

	uint8_t imageDescriptor{};
	sourceImageFile.read((char *) &imageDescriptor, sizeof(uint8_t));
	std::cout << "Image descriptor: " << (int) imageDescriptor << '\n';

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
