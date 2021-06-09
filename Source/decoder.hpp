#ifndef decoder_hpp
#define decoder_hpp

#include <iostream>
#include <fstream>

#include "file_interface.hpp"
/*
#include "image_header.hpp"
#include "image_body.hpp"
*/
#include "image.hpp"
#include "color.hpp"
#include "image_iterator.hpp"

namespace tga
{
	static inline uint8_t scale5BitsTo8Bits(uint8_t v)
	{
		assert(v >= 0 && v < 32);
		return (v << 3) | (v >> 2);
	}

	class Decoder
	{
	public:
		Decoder(FileInterface* file);

		/*
		bool decode(Header& header, Image& image);
		*/
		bool decode(Image& image);

	private:
		bool readHeader(ImageHeader& header);
		//bool readHeader(Image& image);
		//bool readImage(const Header& header, Image& image, Delegate* delegate = nullptr);
		bool readBody(const ImageHeader& header, ImageBody& image);
		void readColorMap(ImageHeader& header);

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
