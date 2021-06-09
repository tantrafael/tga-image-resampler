#include "decoder.hpp"

#include <cassert>
#include <functional>

namespace tga
{
	Decoder::Decoder(FileInterface* file)
		: m_file{ file }
		, m_hasAlpha{ false }
		, m_iterator{}
	{}

	bool Decoder::read(Image& image)
	{
		readHeader(image);

		const auto& header{ image.header };
		auto& body{ image.body };

		body.pixelByteDepth = header.pixelByteDepth();
		body.rowStride = header.width * header.pixelByteDepth();

		const unsigned int bufferSize{ body.rowStride * header.height };
		std::unique_ptr<uint8_t[]> buffer(new uint8_t[bufferSize]);
		body.pixels = buffer.get();

		readBody(image);

		return true;
	}

	bool Decoder::readHeader(Image& image)
	{
		auto& header{ image.header };

		// TODO: Make sure to start at beginning of file.
		header.idLength = read8();
		header.colorMapType = read8();
		header.imageType = static_cast<ImageType>(read8());
		header.colorMapOrigin = read16();
		header.colorMapLength = read16();
		header.colorMapBitDepth = read8();
		header.originX = read16();
		header.originY = read16();
		header.width = read16();
		header.height = read16();
		header.pixelBitDepth = read8();
		header.imageDescriptor = read8();

		// Read ID string.
		if (header.idLength > 0)
		{
			uint8_t byte{};

			for (int i = 0; i < header.idLength; ++i)
			{
				byte = m_file->read8();
				header.imageId.push_back(byte);
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
		std::cout << "X-origin: " << (int) header.originX << '\n';
		std::cout << "Y-origin: " << (int) header.originY << '\n';
		std::cout << "Image width: " << (int) header.width << '\n';
		std::cout << "Image height: " << (int) header.height << '\n';
		std::cout << "Pixel bit depth: " << (int) header.pixelBitDepth << '\n';
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

	bool Decoder::readBody(Image& image)
	{
		m_iterator = ImageIterator{ image };

		const auto& header{ image.header };
		const auto& width{ header.width };
		const auto& height{ header.height };
		color (Decoder::*readPixel)(){};

		switch (header.pixelBitDepth)
		{
			case 8:
				readPixel = &Decoder::read8color;
				break;
			case 15:
			case 16:
				readPixel = &Decoder::read16AsRgb;
				break;
			case 24:
				readPixel = &Decoder::read24AsRgb;
				break;
			case 32:
				readPixel = &Decoder::read32AsRgb;
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

	template<typename T>
	bool Decoder::readImageUncompressed(const int width,
										const int height,
										color (Decoder::*readPixel)())
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

	color Decoder::read8color()
	{
		const auto byte{ read8() };

		return static_cast<color>(byte);
	}

	color Decoder::read16AsRgb()
	{
		const uint16_t v{ read16() };
		auto a{ 255 };

		if (m_hasAlpha)
		{
			if ((v & 0x8000) == 0) // Transparent bit
			{
				a = 0;
			}
		}

		return rgba(scale5BitsTo8Bits((v >> 10) & 0x1F),
					scale5BitsTo8Bits((v >> 5) & 0x1F),
					scale5BitsTo8Bits(v & 0x1F),
					a);
	}

	color Decoder::read24AsRgb()
	{
		const auto b{ read8() };
		const auto g{ read8() };
		const auto r{ read8() };

		return rgba(r, g, b, 255);
	}

	color Decoder::read32AsRgb()
	{
		const auto b{ read8() };
		const auto g{ read8() };
		const auto r{ read8() };
		auto a{ read8() };

		if (!m_hasAlpha)
		{
			a = 255;
		}

		return rgba(r, g, b, a);
	}
}
