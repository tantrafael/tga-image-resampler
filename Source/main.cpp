#include "kernel_type.hpp"
#include "stdio_file_interface.hpp"
#include "image.hpp"
#include "decoder.hpp"
#include "resampler.hpp"
#include "encoder.hpp"

void printUsage(const char* programName) {
	printf("Usage: %s [options]\n", programName);
	printf("  --src [source filename] \t\tSource image to load.\n");
	printf("  --dst [destination filename] \tDestination image to save.\n");
	printf("  --width [integer] \t\t\tSets the width of the output image.\n");
	printf("  --height [integer] \t\t\tSets the height of the output image.\n");
	printf("  --kernel [integer] \t\t\tSpecifies the kernel type.\n");
	printf("      1: Bicubic\n");
	printf("      2: Catmull-Rom\n");
	printf("      3: Mitchell-Netravali\n");
	printf("      4: Cardinal\n");
	printf("      5: B-Spline\n");
	printf("      6: Lanczos\n");
	printf("      7: Lanczos-2\n");
	printf("      8: Lanczos-3\n");
	printf("      9: Lanczos-4\n");
	printf("     10: Lanczos-5\n");
}

int main(int argc, const char* argv[])
{
	std::string src_filename{};
	std::string dst_filename{};
	uint32_t output_width{};
	uint32_t output_height{};
	tga::KernelType kernel{ tga::Bicubic };

	if (argc <= 1)
	{
		printUsage(argv[0]);
		return 0;
	}

	for (int i = 1; i < argc; i++)
	{
		auto optBegin = argv[i];

		for (int j = 0; j < 2; j++)
		{
			(optBegin[0] == '-') ? optBegin++ : optBegin;
		}

		switch (optBegin[0])
		{
			case 's':
				src_filename = argv[++i];
				break;
			case 'd':
				dst_filename = argv[++i];
				break;
			case 'w':
				output_width = atoi(argv[++i]);
				break;
			case 'h':
				output_height = atoi(argv[++i]);
				break;
			case 'k':
				kernel = static_cast<tga::KernelType>(atoi(argv[++i]));
				break;
		}
	}

	// Read source image file.
	//const std::string sourceFilePath{ "/Users/raffa/Work/Star Stable/Sample images/sample_5184×3456.tga" };
	//tga::StdioFileInterface sourceFile{ sourceFilePath, tga::ReadBinary };
	tga::StdioFileInterface sourceFile{ src_filename, tga::ReadBinary };
	tga::Image sourceImage{};
	tga::Decoder decoder{ &sourceFile };
	decoder.read(sourceImage);
	sourceFile.close();

	// Resample image.
	tga::Image targetImage{};
	tga::Resampler resampler{};
	//resampler.resample(sourceImage, 800, 600, tga::BSpline, targetImage);
	resampler.resample(sourceImage, output_width, output_height, kernel, targetImage);

	// Write target image file.
	//const std::string targetFilePath{ "/Users/raffa/Work/Star Stable/resample.tga" };
	//tga::StdioFileInterface targetFile{ targetFilePath, tga::WriteBinary };
	tga::StdioFileInterface targetFile{ dst_filename, tga::WriteBinary };
	tga::Encoder encoder{ &targetFile };
	encoder.write(targetImage);
	targetFile.close();

	return 0;
}
