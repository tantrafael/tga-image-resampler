#ifndef encoder_hpp
#define encoder_hpp

#include "color.hpp"
#include "image.hpp"
#include "image_iterator.hpp"
#include "file_interface.hpp"

namespace tga
{
	class Encoder
	{
	public:
		Encoder(FileInterface* file);

		void write(const Image& image);

	private:
		void writeHeader(const Image& image);
		void writeBody(const Image& image);
		void writeFooter();

		template<typename T>
		bool writeImageUncompressed(const int width,
									const int height,
									void (Encoder::*writePixel)(T));

		void write8(uint8_t value);
		void write16(uint16_t value);
		//void write16(int value);
		void write24AsRgb(color c);

		FileInterface* m_file;
		ImageIterator m_iterator;
	};
}

#endif /* encoder_hpp */
