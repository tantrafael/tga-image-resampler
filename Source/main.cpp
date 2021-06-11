#include "kernel_type.hpp"
#include "stdio_file_interface.hpp"
#include "image.hpp"
#include "decoder.hpp"
#include "resampler.hpp"
#include "encoder.hpp"

void printUsage(const char* programName) {
	printf("Usage: %s [options]\n", programName);
	printf("  --src [source filename] \tSource TGA image to load.\n");
	printf("  --dst [destination filename] \tDestination TGA image to save.\n");
	printf("  --width [integer] \t\tSets the width of the output image.\n");
	printf("  --height [integer] \t\tSets the height of the output image.\n");
	printf("  --kernel [integer] \t\tSpecifies the kernel type.\n");
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

void readUserInput(const int argc,
				   const char* argv[],
				   std::string& sourceFileName,
				   std::string& destinationFileName,
				   uint32_t& outputWidth,
				   uint32_t& outputHeight,
				   tga::KernelType& kernel)
{
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
				sourceFileName = argv[++i];
				break;
			case 'd':
				destinationFileName = argv[++i];
				break;
			case 'w':
				outputWidth = atoi(argv[++i]);
				break;
			case 'h':
				outputHeight = atoi(argv[++i]);
				break;
			case 'k':
				kernel = static_cast<tga::KernelType>(atoi(argv[++i]));
				break;
		}
	}
}

int main(int argc, const char* argv[])
{
	std::string sourceFileName{};
	std::string destinationFileName{};
	uint32_t outputWidth{};
	uint32_t outputHeight{};
	tga::KernelType kernel{ tga::Lanczos };

	if (argc <= 1)
	{
		printUsage(argv[0]);
		return 0;
	}

	readUserInput(argc,
				  argv,
				  sourceFileName,
				  destinationFileName,
				  outputWidth,
				  outputHeight,
				  kernel);

	// Read source file.
	printf("Reading source file.\n");

	tga::StdioFileInterface sourceFile{ sourceFileName, tga::ReadBinary };
	tga::Image sourceImage{};
	tga::Decoder decoder{ &sourceFile };

	if (!decoder.read(sourceImage))
	{
		printf("Error reading source file.\n");
		return 0;
	}

	sourceFile.close();

	printf("TGA image type: %i.\n", sourceImage.header.imageType);
	printf("Dimensions: %ix%i px.\n", sourceImage.header.width, sourceImage.header.height);
	printf("Pixel bit depth: %i.\n", sourceImage.header.pixelBitDepth);

	// Resample image.
	printf("Resampling image using kernel %i, destination dimensions %ix%i px.\n",
		   kernel, outputWidth, outputHeight);

	tga::Image destinationImage{};
	tga::Resampler resampler{};

	if (!resampler.resample(sourceImage,
							outputWidth,
							outputHeight,
							kernel,
							destinationImage))
	{
		printf("Error resampling image.\n");
		return 0;
	}

	// Write destination file.
	printf("Writing destination file.\n");

	tga::StdioFileInterface destinationFile{ destinationFileName, tga::WriteBinary };
	tga::Encoder encoder{ &destinationFile };
	encoder.write(destinationImage);

	destinationFile.close();

	return 0;
}
