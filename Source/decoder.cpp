#include "decoder.hpp"

#include <cassert>

namespace tga
{
	//Decoder::Decoder(std::ifstream &file)
	//Decoder::Decoder(std::ifstream* file)
	Decoder::Decoder(FileInterface* file)
		: m_file{ file }
	{
		/*
		auto sourceImageFileSize = sourceImageFile.tellg();
		std::vector<char> sourceImageBuffer(sourceImageFileSize);
		sourceImageFile.seekg(0);
		sourceImageFile.read(sourceImageBuffer.data(), sourceImageFileSize);

		std::cout << "Source image file read.\n";
		*/
	}

	bool Decoder::readHeader(Header& header)
	{
		//m_file->seekg(0);

		header.idLength = read8();
		header.colorMapType = read8();
		header.imageType = read8();
		header.colorMapOrigin = read16();
		header.colorMapLength = read16();
		header.colorMapDepth = read8();
		header.xOrigin = read16();
		header.yOrigin = read16();
		header.width = read16();
		header.height = read16();
		header.pixelDepth = read8();
		header.imageDescriptor = read8();

		// Read ID string.
		if (header.idLength > 0)
		{
			// TODO: Rename and declare variable outside loop.
			for (uint8_t i = 0; i < header.idLength; ++i)
			{
				//uint8_t chr{};
				//m_file->read((char*) &chr, sizeof(uint8_t));
				uint8_t chr{ m_file->read8() };
				header.imageId.push_back(chr);
			}
		}

		// Read color map.
		/*
		if (header.colorMapType == 1)
		{
			readColorMap(header);
		}
		*/

		std::cout << "ID length: " << (int) header.idLength << '\n';
		std::cout << "Color map type: " << (int) header.colorMapType << '\n';
		std::cout << "Image type: " << (int) header.imageType << '\n';
		std::cout << "Color map origin: " << (int) header.colorMapOrigin << '\n';
		std::cout << "Color map length: " << (int) header.colorMapLength << '\n';
		std::cout << "Color map depth: " << (int) header.colorMapDepth << '\n';
		std::cout << "X-origin: " << (int) header.xOrigin << '\n';
		std::cout << "Y-origin: " << (int) header.yOrigin << '\n';
		std::cout << "Image width: " << (int) header.width << '\n';
		std::cout << "Image height: " << (int) header.height << '\n';
		std::cout << "Pixel depth: " << (int) header.pixelDepth << '\n';
		std::cout << "Image descriptor: " << (int) header.imageDescriptor << '\n';

		return true;
	}

	/*
	void Decoder::readColorMap(Header& header)
	{
		header.colorMap = ColorMap{ header.colorMapLength };
		
		for (int i = 0; i < header.colorMapLength; ++i)
		{
			switch (header.colorMapDepth)
			{
				case 15:
				case 16:
				case 24:
					break;
				case 32:
					break;
			}
		}
	}
	*/

	bool Decoder::readImage(const Header& header, Image& image)
	{
		m_iterator = ImageIterator{ header, image };

		// TODO: Clean up. Use foreach.
		for (int y = 0; y < header.height; ++y)
		{
			switch (header.imageType)
			{
				case UncompressedTrueColor:
					switch (header.pixelDepth)
					{
						case 15:
						case 16:
						case 24:
							if (readUncompressedData<uint32_t>(header.width, &Decoder::read24AsRgb))
							{
								return true;
							}
							break;
						case 32:
						default:
							assert(false);
							break;
					}

					break;
			}
		}

		return true;
	}

	template<typename T>
	bool Decoder::readUncompressedData(const int w, uint32_t (Decoder::*readPixel)())
	{
		// TODO: Use foreach.
		for (int x = 0; x < w; ++x)
		{
			T value = static_cast<T>((this->*readPixel)());

			if (m_iterator.putPixel<T>(value))
			{
				return true;
			}
		}

		return false;
	}

	uint8_t Decoder::read8()
	{
		return m_file->read8();
	}

	// Read 16 bits using in little-endian byte ordering.
	uint16_t Decoder::read16()
	{
		// TODO: List initialize.
		uint8_t b1 = m_file->read8();
		uint8_t b2 = m_file->read8();

		if (m_file->ok())
		{
			return ((b2 << 8) | b1); // Little endian
		}
		else
		{
			return 0;
		}
	}

	color Decoder::read24AsRgb()
	{
		// TODO: List initialize.
		const uint8_t b = read8();
		const uint8_t g = read8();
		const uint8_t r = read8();

		return rgba(r, g, b, 255);
	}

}
