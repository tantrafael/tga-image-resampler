#include <vector>

#include "stdio_file_interface.hpp"
#include "image.hpp"
#include "decoder.hpp"
#include "resampler.hpp"
#include "encoder.hpp"

int main(int argc, const char * argv[]) {
	// Read source image file.
	auto sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_5184×3456.tga" };

	/*
	FILE* f0 = std::fopen(sourceFilePath, "rb");
	tga::StdioFileInterface sourceFile{ f0 };
	*/

	//tga::StdioFileInterface sourceFile{ sourceFilePath, 0 };
	tga::StdioFileInterface sourceFile{};
	sourceFile.open(sourceFilePath, true);

	tga::Image sourceImage{};
	tga::Decoder decoder{ &sourceFile };
	decoder.read(sourceImage);

	//std::fclose(f0);
	sourceFile.close();

	// Resample image.
	tga::Image targetImage{};
	tga::Resampler resampler{};
	resampler.resample(sourceImage, 800, 600, tga::BSpline, targetImage);

	// Write target image file.
	auto targetFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };

	/*
	FILE* f1 = std::fopen(targetFilePath, "wb");
	tga::StdioFileInterface targetFile{ f1 };

	tga::Encoder encoder{ &targetFile };
	encoder.write(targetImage);

	std::fclose(f1);
	*/

	tga::StdioFileInterface targetFile{};
	targetFile.open(targetFilePath, false);

	tga::Encoder encoder{ &targetFile };
	encoder.write(targetImage);

	targetFile.close();

	return 0;
}
