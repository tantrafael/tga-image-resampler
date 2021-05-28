#ifndef encoder_hpp
#define encoder_hpp

#include "file_interface.hpp"
#include "header.hpp"
#include "image.hpp"
#include "color.hpp"
#include "image_iterator.hpp"

namespace tga
{
	class Encoder
	{
	public:
		Encoder(FileInterface* file);

		void writeHeader(const Header& header);
		void writeImage(const Header& header, const Image& image);

	private:
		template<typename T>
		bool writeImageUncompressed(const int width,
									const int height,
									void (Encoder::*writePixel)(T));

		void write8(uint8_t value);
		void write16(uint16_t value);
		void write24AsRgb(color c);

		FileInterface* m_file;
		ImageIterator m_iterator;
	};
}

#endif /* encoder_hpp */
