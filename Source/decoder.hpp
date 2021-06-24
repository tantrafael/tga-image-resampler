#ifndef decoder_hpp
#define decoder_hpp

#include <iostream>

#include "color.hpp"
#include "image.hpp"
#include "image_iterator.hpp"
#include "file_interface.hpp"

namespace tga
{
	static inline int scale5BitsTo8Bits(int v)
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
		void readColorMap(Image& image);

		template<typename T>
		void readImageUncompressed(const int width,
								   const int height,
								   color (Decoder::*readPixel)());

		int read8();
		int read16();

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
