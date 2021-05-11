#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, const char * argv[]) {
	std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/earth.tga" };
	std::string targetImageFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };

	// Read source image file.
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

	// Write target image file.
	std::ofstream targetImageFile{ targetImageFilePath, std::ifstream::binary };

	if (!targetImageFile)
	{
		std::cerr << "Target image file could not be opened for writing.\n";
		return 1;
	}

	targetImageFile.write(sourceImageBuffer.data(), sourceImageFileSize);

	std::cout << "Target image file written.\n";

	return 0;
}
