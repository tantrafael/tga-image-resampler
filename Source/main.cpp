#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, const char * argv[]) {
	std::string sourceImageFilePath{ "/Users/raffa/Work/Star Stable/Sample images/earth.tga" };
	std::ifstream sourceImageFile{ sourceImageFilePath, std::ifstream::binary|std::ifstream::ate };

	if (!sourceImageFile)
	{
		std::cerr << "Source image file could not be opened for reading.\n";
		return 1;
	}
 
	auto sourceImageFileSize = sourceImageFile.tellg();
	std::vector<char> sourceImageBuffer(sourceImageFileSize);

	sourceImageFile.read(sourceImageBuffer.data(), sourceImageFileSize);

	std::cout << sourceImageBuffer.size() << '\n';

	return 0;
}
