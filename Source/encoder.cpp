#include "encoder.hpp"

#include <cassert>

namespace tga
{
	Encoder::Encoder(FileInterface* file)
		: m_file{ file }
	{}

	void Encoder::writeHeader(const Header &header)
	{
		write8(header.idLength);
		write8(header.colorMapType);
		write8(header.imageType);
		write16(header.colorMapOrigin);
		write16(header.colorMapLength);
		write8(header.colorMapBitDepth);
		write16(header.xOrigin);
		write16(header.yOrigin);
		write16(header.width);
		write16(header.height);
		write8(header.pixelBitDepth);
		write8(header.imageDescriptor);

		assert(header.colorMapLength == 0);
	}

	void Encoder::writeImage(const Header &header, const Image &image)
	{
		m_iterator = ImageIterator{ header, const_cast<Image&>(image) };

		const auto width{ header.width };
		const auto height{ header.height };
		void (Encoder::*writePixel)(color){};

		switch (header.pixelBitDepth)
		{
			case 8:
				//writePixel = &Encoder::read8color;
				break;
			case 15:
			case 16:
				//writePixel = &Encoder::read16AsRgb;
				break;
			case 24:
				writePixel = &Encoder::write24AsRgb;
				break;
			case 32:
				//writePixel = &Encoder::read32AsRgb;
				break;
		}

		switch (header.imageType)
		{
			case NoImageData:
				break;
			case UncompressedColorMapped:
			case UncompressedGrayscale:
				//writeImageUncompressed<uint8_t>(width, height, writePixel);
				break;
			case UncompressedTrueColor:
				writeImageUncompressed<uint32_t>(width, height, writePixel);
				break;
			case RunLengthEncodedColorMapped:
			case RunLengthEncodedGrayscale:
				//readImageRunLengthEncoded<uint8_t>(width, height, readPixel);
				break;
			case RunLengthEncodedTrueColor:
				//readImageRunLengthEncoded<uint32_t>(width, height, readPixel);
				break;
		}
	}

	template<typename T>
	bool Encoder::writeImageUncompressed(const int width,
										 const int height,
										 void (Encoder::*writePixel)(T))
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				(this->*writePixel)(m_iterator.getPixel<T>());
			}
		}

		return true;
	}

	void Encoder::write8(uint8_t value)
	{
		m_file->write8(value);
	}

	void Encoder::write16(uint16_t value)
	{
		// Little endian
		m_file->write8(value & 0x00FF);
		m_file->write8((value & 0xFF00) >> 8);
	}

	void Encoder::write24AsRgb(color c)
	{
		write8(getb(c));
		write8(getg(c));
		write8(getr(c));
	}
}
