#ifndef decoder_hpp
#define decoder_hpp

#include <iostream>
#include <fstream>

#include "header.hpp"

namespace tga
{
	class Decoder
	{
	public:
		//Decoder(std::ifstream& file);
		Decoder(std::ifstream* file);

		bool readHeader(Header& header);

	private:
		std::ifstream* m_file;
	};
}

#endif /* decoder_hpp */
