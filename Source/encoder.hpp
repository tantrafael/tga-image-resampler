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
									void (Encoder::*writePixel)(color));

		void write8(const int value);
		void write16(const int value);

		void write8color(const color c);
		void write24AsRgb(const color c);

		FileInterface* m_file;
		ImageIterator m_iterator;
	};
}

#endif /* encoder_hpp */
