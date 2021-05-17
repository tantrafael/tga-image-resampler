#include "decoder.hpp"

namespace tga
{
	//Decoder::Decoder(std::ifstream &file)
	Decoder::Decoder(std::ifstream* file)
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
		m_file->seekg(0);

		m_file->read((char*) &header.idLength, sizeof(uint8_t));
		m_file->read((char*) &header.colorMapType, sizeof(uint8_t));
		m_file->read((char*) &header.imageType, sizeof(uint8_t));
		m_file->read((char*) &header.colorMapOrigin, sizeof(uint16_t));
		m_file->read((char*) &header.colorMapLength, sizeof(uint16_t));
		m_file->read((char*) &header.colorMapDepth, sizeof(uint8_t));
		m_file->read((char*) &header.xOrigin, sizeof(uint16_t));
		m_file->read((char*) &header.yOrigin, sizeof(uint16_t));
		m_file->read((char*) &header.width, sizeof(uint16_t));
		m_file->read((char*) &header.height, sizeof(uint16_t));
		m_file->read((char*) &header.pixelDepth, sizeof(uint8_t));
		m_file->read((char*) &header.imageDescriptor, sizeof(uint8_t));

		// Read ID string.
		if (header.idLength > 0)
		{
			for (uint8_t i = 0; i < header.idLength; ++i)
			{
				uint8_t chr{};
				m_file->read((char*) &chr, sizeof(uint8_t));
				header.imageId.push_back(chr);
			}
		}

		// Read color map.
		/*
		if (header.colormapType == 1)
			readColormap(header);
		*/

		if (header.colorMapType == 1)
		{
			readColorMap(header);
		}


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

	bool Decoder::readImage(const Header& header, Image& image)
	{
		m_iterator = ImageIterator{ header, image };

		for (int y = 0; y < header.height; ++y)
		{
			switch (header.imageType)
			{
				case 16:
					
					break;
			}
		}

		return true;
	}

	template<typename T>
	bool Decoder::readUncompressedData(const int w, uint32_t (Decoder::*readPixel)())
	{
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
}
