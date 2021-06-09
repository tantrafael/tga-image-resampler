#include <vector>

#include "stdio_file_interface.hpp"
#include "decoder.hpp"
#include "resampler.hpp"
#include "encoder.hpp"
#include "image.hpp"
/*
#include "image_header.hpp"
#include "image_body.hpp"
*/

int main(int argc, const char * argv[]) {
	// Read source image file.
	auto sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_5184×3456.tga" };
	FILE* f0 = std::fopen(sourceFilePath, "rb");
	tga::StdioFileInterface sourceFile{ f0 };

	/*
	tga::Header sourceHeader{};
	tga::Image sourceImage{};
	*/
	tga::Image sourceImage{};
	tga::Decoder decoder{ &sourceFile };

	/*
	decoder.decode(sourceHeader, sourceImage);
	*/
	decoder.decode(sourceImage);

	std::fclose(f0);

	// Resample image.
	/*
	tga::Header targetHeader{};
	tga::Image targetImage{};
	*/
	tga::Image targetImage{};
	tga::Resampler resampler{};

	/*
	resampler.resample(sourceHeader,
					   sourceImage,
					   400,
					   300,
					   tga::BSpline,
					   targetHeader,
					   targetImage);
	*/

	resampler.resample(sourceImage, 400, 300, tga::BSpline, targetImage);

	// Write target image file.
	auto targetFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };
	FILE* f1 = std::fopen(targetFilePath, "wb");
	tga::StdioFileInterface targetFile{ f1 };

	tga::Encoder encoder{ &targetFile };

	/*
	encoder.encode(targetHeader, targetImage);
	*/
	encoder.encode(targetImage);

	std::fclose(f1);

	return 0;
}
