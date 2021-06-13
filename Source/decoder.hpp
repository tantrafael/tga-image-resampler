#ifndef decoder_hpp
#define decoder_hpp

#include <iostream>

#include "color.hpp"
#include "image.hpp"
#include "image_iterator.hpp"
#include "file_interface.hpp"

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

		bool read(Image& image);

	private:
		bool readHeader(Image& image);
		bool readBody(Image& image);
		void readColorMap(ImageHeader& header);

		template<typename T>
		void readImageUncompressed(const int width,
								   const int height,
								   color (Decoder::*readPixel)());

		//uint8_t read8();
		int read8();
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
