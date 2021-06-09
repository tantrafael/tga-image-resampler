#ifndef encoder_hpp
#define encoder_hpp

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
	class Encoder
	{
	public:
		Encoder(FileInterface* file);

		//void encode(const ImageHeader& header, const ImageBody& image);
		void encode(const Image& image);

	private:
		void writeHeader(const ImageHeader& header);
		void writeBody(const ImageHeader& header, const ImageBody& image);

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
