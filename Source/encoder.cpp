#include "encoder.hpp"

#include <cassert>

namespace tga
{
	Encoder::Encoder(FileInterface* file)
		: m_file{ file }
		, m_hasAlpha{ false }
		, m_iterator{}
	{}

	void Encoder::write(const Image& image)
	{
		writeHeader(image);
		writeBody(image);
		writeFooter();
	}

	void Encoder::writeHeader(const Image &image)
	{
		write8(image.idLength);
		write8(image.colorMapType);
		write8(image.imageType);
		write16(image.colorMapOrigin);
		write16(image.colorMapLength);
		write8(image.colorMapBitDepth);
		write16(image.originX);
		write16(image.originY);
		write16(image.width);
		write16(image.height);
		write8(image.pixelBitDepth);
		write8(image.imageDescriptor);

		m_hasAlpha = (image.pixelBitDepth == 16
					  || image.pixelBitDepth == 32);

		assert(image.colorMapLength == 0);
	}

	void Encoder::writeBody(const Image &image)
	{
		m_iterator = ImageIterator{ const_cast<Image&>(image) };

		//const auto& header{ image.header };
		const auto& width{ image.width };
		const auto& height{ image.height };
		void (Encoder::*writePixel)(color){};

		switch (image.pixelBitDepth)
		{
			case 8:
				writePixel = &Encoder::write8color;
				break;
			case 15:
			case 16:
				writePixel = &Encoder::write16AsRgb;
				break;
			case 24:
				writePixel = &Encoder::write24AsRgb;
				break;
			case 32:
				writePixel = &Encoder::write32AsRgb;
				break;
		}

		switch (image.imageType)
		{
			case NoImageData:
				break;
			case UncompressedColorMapped:
			case UncompressedGrayscale:
				writeImageUncompressed<uint8_t>(width, height, writePixel);
				break;
			case UncompressedTrueColor:
				writeImageUncompressed<uint32_t>(width, height, writePixel);
				break;
			case RunLengthEncodedColorMapped:
			case RunLengthEncodedGrayscale:
				//writeImageRunLengthEncoded<uint8_t>(width, height, writePixel);
				break;
			case RunLengthEncodedTrueColor:
				//writeImageRunLengthEncoded<uint32_t>(width, height, writePixel);
				break;
		}
	}

	void Encoder::writeFooter()
	{
		std::string tga2Footer{ "\0\0\0\0\0\0\0\0TRUEVISION-XFILE.\0" };

		for (int i = 0; i < 26; ++i) {
			write8(tga2Footer[i]);
		}
	}

	template<typename T>
	bool Encoder::writeImageUncompressed(const int width,
										 const int height,
										 void (Encoder::*writePixel)(color))
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const auto pixelValue{ m_iterator.getPixel<T>() };
				const auto colorValue{ static_cast<color>(pixelValue) };
				(this->*writePixel)(colorValue);
			}
		}

		return true;
	}

	void Encoder::write8(const int value)
	{
		m_file->write8(value);
	}

	void Encoder::write16(const int value)
	{
		// Little endian
		m_file->write8(value & 0x00FF);
		m_file->write8((value & 0xFF00) >> 8);
	}

	void Encoder::write8color(const color c)
	{
		write8(static_cast<int>(c));
	}

	void Encoder::write16AsRgb(const color c)
	{
		const auto r{ getR(c) };
		const auto g{ getG(c) };
		const auto b{ getB(c) };
		const auto a{ getA(c) };

		const auto value = (((r>>3) << 10)
							| ((g>>3) << 5)
							| ((b>>3))
							| (m_hasAlpha && a >= 128 ? 0x8000 : 0)
							);

		write16(value);
	}

	void Encoder::write24AsRgb(const color c)
	{
		write8(getB(c));
		write8(getG(c));
		write8(getR(c));
	}

	void Encoder::write32AsRgb(const color c)
	{
		write8(getB(c));
		write8(getG(c));
		write8(getR(c));
		write8(getA(c));
	}
}
