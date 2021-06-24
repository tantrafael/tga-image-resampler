#include "decoder.hpp"

namespace tga
{
	Decoder::Decoder(FileInterface* file)
		: m_file{ file }
		, m_hasAlpha{ false }
		, m_iterator{}
	{}

	bool Decoder::read(Image& image)
	{
		if (!readHeader(image))
		{
			return false;
		}

		image.pixelByteDepth = image.getPixelByteDepth();
		image.rowStride = image.width * image.pixelByteDepth;

		const auto bufferSize{ image.rowStride * image.height };
		std::unique_ptr<uint8_t[]> buffer(new uint8_t[bufferSize]);
		image.pixels = buffer.get();

		if (!readBody(image))
		{
			return false;
		}

		return true;
	}

	bool Decoder::readHeader(Image& image)
	{
		image.idLength = read8();
		image.colorMapType = read8();
		image.imageType = static_cast<ImageType>(read8());
		image.colorMapOrigin = read16();
		image.colorMapLength = read16();
		image.colorMapBitDepth = read8();
		image.originX = read16();
		image.originY = read16();
		image.width = read16();
		image.height = read16();
		image.pixelBitDepth = read8();
		image.imageDescriptor = read8();

		// Read ID string.
		if (image.idLength > 0)
		{
			for (int i = 0; i < image.idLength; ++i)
			{
				const auto byte{ m_file->read8() };
				image.imageId.push_back(byte);
			}
		}

		// Read color map.
		if (image.colorMapType == 1)
		{
			readColorMap(image);
		}

		return image.validate();
	}

	void Decoder::readColorMap(Image& image)
	{
		image.colorMap = ColorMap{ image.colorMapLength };
		// TODO: Read color map.
	}

	bool Decoder::readBody(Image& image)
	{
		m_iterator = ImageIterator{ image };

		//const auto& header{ image.header };
		const auto& width{ image.width };
		const auto& height{ image.height };
		color (Decoder::*readPixel)(){};

		switch (image.pixelBitDepth)
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

		switch (image.imageType)
		{
			case NoImageData:
				return false;
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
	void Decoder::readImageUncompressed(const int width,
										const int height,
										color (Decoder::*readPixel)())
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const auto colorValue{ (this->*readPixel)() };
				const auto pixelValue{ static_cast<T>(colorValue) };
				m_iterator.putPixel<T>(pixelValue);
			}
		}
	}

	int Decoder::read8()
	{
		return m_file->read8();
	}

	// Read 16 bits using little-endian byte ordering.
	int Decoder::read16()
	{
		const auto b1{ m_file->read8() };
		const auto b2{ m_file->read8() };

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
		const int v{ read16() };
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
