#ifndef decoder_hpp
#define decoder_hpp

#include <iostream>
#include <fstream>

#include "header.hpp"
#include "image.hpp"
#include "image_iterator.hpp"

namespace tga
{
	class Decoder
	{
	public:
		//Decoder(std::ifstream& file);
		Decoder(std::ifstream* file);

		bool readHeader(Header& header);
		//bool readImage(const Header& header, Image& image, Delegate* delegate = nullptr);
		bool readImage(const Header& header, Image& image);

	private:
		void readColorMap(Header& header);

		template<typename T>
		bool readUncompressedData(const int w, uint32_t (Decoder::*readPixel)());

		std::ifstream* m_file;
		ImageIterator m_iterator;
	};
}

#endif /* decoder_hpp */
