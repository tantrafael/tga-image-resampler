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
	class Decoder
	{
	public:
		//Decoder(std::ifstream& file);
		//Decoder(std::ifstream* file);
		Decoder(FileInterface* file);

		bool readHeader(Header& header);
		//bool readImage(const Header& header, Image& image, Delegate* delegate = nullptr);
		bool readImage(const Header& header, Image& image);

	private:
		void readColorMap(Header& header);

		bool readImageRow(const ImageType type,
			   const uint8_t pixelBitDepth,
			   const uint16_t width);

		bool readImageRowUncompressedTrueColor(const uint8_t pixelBitDepth,
											   const uint8_t width);

		template<typename T>
		bool readUncompressedData(const int w, uint32_t (Decoder::*readPixel)());

		uint8_t read8();
		uint16_t read16();

		color read24AsRgb();

		FileInterface* m_file;
		ImageIterator m_iterator;
	};
}

#endif /* decoder_hpp */
