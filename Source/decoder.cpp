#include "decoder.hpp"

namespace tga
{
	//Decoder::Decoder(std::ifstream &file)
	Decoder::Decoder(std::ifstream* file)
		: m_file{ file }
	{
		/*
		auto sourceImageFileSize = sourceImageFile.tellg();
		std::vector<char> sourceImageBuffer(sourceImageFileSize);
		sourceImageFile.seekg(0);
		sourceImageFile.read(sourceImageBuffer.data(), sourceImageFileSize);

		std::cout << "Source image file read.\n";
		*/
	}

	bool Decoder::readHeader(Header& header)
	{
		m_file->seekg(0);

		m_file->read((char*) &header.idLength, sizeof(uint8_t));
		m_file->read((char*) &header.colorMapType, sizeof(uint8_t));
		m_file->read((char*) &header.imageType, sizeof(uint8_t));
		m_file->read((char*) &header.colorMapOrigin, sizeof(uint16_t));
		m_file->read((char*) &header.colorMapLength, sizeof(uint16_t));
		m_file->read((char*) &header.colorMapDepth, sizeof(uint8_t));
		m_file->read((char*) &header.xOrigin, sizeof(uint16_t));
		m_file->read((char*) &header.yOrigin, sizeof(uint16_t));
		m_file->read((char*) &header.width, sizeof(uint16_t));
		m_file->read((char*) &header.height, sizeof(uint16_t));
		m_file->read((char*) &header.pixelDepth, sizeof(uint8_t));
		m_file->read((char*) &header.imageDescriptor, sizeof(uint8_t));

		std::cout << "ID length: " << (int) header.idLength << '\n';
		std::cout << "Color map type: " << (int) header.colorMapType << '\n';
		std::cout << "Image type: " << (int) header.imageType << '\n';
		std::cout << "Color map origin: " << (int) header.colorMapOrigin << '\n';
		std::cout << "Color map length: " << (int) header.colorMapLength << '\n';
		std::cout << "Color map depth: " << (int) header.colorMapDepth << '\n';
		std::cout << "X-origin: " << (int) header.xOrigin << '\n';
		std::cout << "Y-origin: " << (int) header.yOrigin << '\n';
		std::cout << "Image width: " << (int) header.width << '\n';
		std::cout << "Image height: " << (int) header.height << '\n';
		std::cout << "Pixel depth: " << (int) header.pixelDepth << '\n';
		std::cout << "Image descriptor: " << (int) header.imageDescriptor << '\n';

		return true;
	}
}
