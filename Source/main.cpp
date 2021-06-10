#include <vector>

#include "stdio_file_interface.hpp"
#include "image.hpp"
#include "decoder.hpp"
#include "resampler.hpp"
#include "encoder.hpp"

int main(int argc, const char * argv[]) {
	// Read source image file.
	auto sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_5184×3456.tga" };

	//tga::StdioFileInterface sourceFile{ sourceFilePath, 0 };
	tga::StdioFileInterface sourceFile{};
	sourceFile.open(sourceFilePath, tga::ReadBinary);

	tga::Image sourceImage{};
	tga::Decoder decoder{ &sourceFile };
	decoder.read(sourceImage);

	sourceFile.close();

	// Resample image.
	tga::Image targetImage{};
	tga::Resampler resampler{};
	resampler.resample(sourceImage, 800, 600, tga::BSpline, targetImage);

	// Write target image file.
	auto targetFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };

	tga::StdioFileInterface targetFile{};
	targetFile.open(targetFilePath, tga::WriteBinary);

	tga::Encoder encoder{ &targetFile };
	encoder.write(targetImage);

	targetFile.close();

	return 0;
}
