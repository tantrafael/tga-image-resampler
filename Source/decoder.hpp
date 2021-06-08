#ifndef decoder_hpp
#define decoder_hpp

#include <iostream>
#include <fstream>

#include "file_interface.hpp"
#include "header.hpp"
#include "image.hpp"
#include "color.hpp"
#include "image_iterator.hpp"

namespace tga
{
	static inline uint8_t scale_5bits_to_8bits(uint8_t v)
	{
		assert(v >= 0 && v < 32);
		return (v << 3) | (v >> 2);
	}

	class Decoder
	{
	public:
		Decoder(FileInterface* file);

		bool decode(Header& header, Image& image);
		bool readHeader(Header& header);
		//bool readImage(const Header& header, Image& image, Delegate* delegate = nullptr);
		bool readImage(const Header& header, Image& image);

	private:
		void readColorMap(Header& header);

		template<typename T>
		bool readImageUncompressed(const int width,
								   const int height,
								   uint32_t (Decoder::*readPixel)());

		uint8_t read8();
		uint16_t read16();

		color read8color();
		color read16AsRgb();
		color read24AsRgb();
		color read32AsRgb();

		FileInterface* m_file;
		bool m_hasAlpha;
		ImageIterator m_iterator;
	};
}

#endif /* decoder_hpp */
