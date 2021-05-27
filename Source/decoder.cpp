#include "decoder.hpp"

#include <cassert>
#include <functional>

namespace tga
{
	//Decoder::Decoder(std::ifstream &file)
	//Decoder::Decoder(std::ifstream* file)
	Decoder::Decoder(FileInterface* file)
		: m_file{ file }
	{}

	bool Decoder::readHeader(Header& header)
	{
		//m_file->seekg(0);

		header.idLength = read8();
		header.colorMapType = read8();
		//header.imageType = read8();
		header.imageType = static_cast<ImageType>(read8());
		header.colorMapOrigin = read16();
		header.colorMapLength = read16();
		header.colorMapBitDepth = read8();
		header.xOrigin = read16();
		header.yOrigin = read16();
		header.width = read16();
		header.height = read16();
		header.pixelBitDepth = read8();
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

		// TODO: Clean up type cast.
		std::cout << "ID length: " << (int) header.idLength << '\n';
		std::cout << "Color map type: " << (int) header.colorMapType << '\n';
		std::cout << "Image type: " << (int) header.imageType << '\n';
		std::cout << "Color map origin: " << (int) header.colorMapOrigin << '\n';
		std::cout << "Color map length: " << (int) header.colorMapLength << '\n';
		std::cout << "Color map depth: " << (int) header.colorMapBitDepth << '\n';
		std::cout << "X-origin: " << (int) header.xOrigin << '\n';
		std::cout << "Y-origin: " << (int) header.yOrigin << '\n';
		std::cout << "Image width: " << (int) header.width << '\n';
		std::cout << "Image height: " << (int) header.height << '\n';
		std::cout << "Pixel depth: " << (int) header.pixelBitDepth << '\n';
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

		//color (Decoder::*foo)() = &Decoder::read24AsRgb;

		// 15:
		// 16:
		// 24: &Decoder::read24AsRgb
		// 32:

		//bool (Decoder::*foo)(uint8_t, uint16_t){ &Decoder::readImageRowUncompressedTrueColor };
		//std::function<bool(uint8_t, uint16_t)> fcnPtr{ &Decoder::readImageRowUncompressedTrueColor };
		//std::function<bool(const int, uint32_t)> fcnPtr = &Decoder::readUncompressedData;
		//auto fcnPtr = &Decoder::read8;
		//auto fcnPtr = &Decoder::readUncompressedData;
		//auto lambda = []<typename T>(std::vector<T> t){};

		/*
		for (int y = 0; y < header.height; ++y)
		{
			//readImageRow(header.imageType, header.pixelBitDepth, header.width);
			//(*foo)(header.pixelBitDepth, header.width);
			//readUncompressedData<uint32_t>(header.width, &Decoder::read24AsRgb);
		}
		*/

		const auto width{ header.width };
		const auto height{ header.height };
		//const auto imageType { header.imageType };
		//const uint8_t pixelBitDepth{ header.pixelBitDepth };
		auto readPixel { &Decoder::read24AsRgb };
		//uint32_t (Decoder::*readPixel)() { &Decoder::read24AsRgb };

		//readImageUncompressed<uint32_t>(width, height, readPixel);

		switch (header.pixelBitDepth)
		{
			case 8:
				//readPixel = &Decoder::read8Color;
				break;
			case 15:
			case 16:
				//readPixel = &Decoder::read16AsRgb;
				break;
			case 24:
				readPixel = &Decoder::read24AsRgb;
				break;
			case 32:
				//readPixel = &Decoder::read32AsRgb;
				break;
		}

		switch (header.imageType)
		{
			case NoImageData:
				break;
			case UncompressedColorMapped:
			case UncompressedGrayscale:
				readImageUncompressed<uint8_t>(width, height, readPixel);
				break;
			case UncompressedTrueColor:
				readImageUncompressed<uint32_t>(width, height, readPixel);
				break;
			case RunLengthEncodedColorMapped:
			case RunLengthEncodedGrayscale:
				//readImageRunLengthEncoded<uint8_t>(width, height, readPixel);
				break;
			case RunLengthEncodedTrueColor:
				//readImageRunLengthEncoded<uint32_t>(width, height, readPixel);
				break;
		}

		return true;
	}

	/*
	bool Decoder::readImageRowUncompressedTrueColor(const uint8_t pixelBitDepth,
													const uint16_t width)
	{
		switch (pixelBitDepth)
		{
			case 15:
			case 16:
			case 24:
				if (!readUncompressedData<uint32_t>(width, &Decoder::read24AsRgb))
				{
					return false;
				}
				break;
			case 32:
			default:
				assert(false);
				break;
		}

		return true;
	}
	*/

	template<typename T>
	bool Decoder::readImageUncompressed(const int width,
										const int height,
										uint32_t (Decoder::*readPixel)())
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				T value = static_cast<T>((this->*readPixel)());

				if (!m_iterator.putPixel<T>(value))
				{
					return false;
				}
			}
		}

		return true;
	}

	/*
	template<typename T>
	bool Decoder::readUncompressedData(const int w, uint32_t (Decoder::*readPixel)())
	{
		for (int x = 0; x < w; ++x)
		{
			T value = static_cast<T>((this->*readPixel)());

			if (!m_iterator.putPixel<T>(value))
			{
				return false;
			}
		}

		return true;
	}
	*/

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
