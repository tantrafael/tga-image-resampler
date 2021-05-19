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
		const int w{ header.width };
		const int h{ header.height };

		// TODO: Question const cast.
		m_iterator = ImageIterator(header, const_cast<Image&>(image));

		// TODO: Clean up.
		switch (header.imageType)
		{
			case UncompressedTrueColor:
				switch (header.pixelBitDepth)
				{
					case 24:
							for (int y = 0; y < h; ++y)
							{
								for (int x = 0; x < w; ++x)
								{
									write24AsRgb(m_iterator.getPixel<uint32_t>());
								}
							}
						break;
				}
				break;
		}
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
