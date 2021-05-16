#ifndef header_hpp
#define header_hpp

namespace tga
{
	struct Header
	{
		uint8_t  idLength;
		uint8_t  colorMapType;
		uint8_t  imageType;
		uint16_t colorMapOrigin;
		uint16_t colorMapLength;
		uint8_t  colorMapDepth;
		uint16_t xOrigin;
		uint16_t yOrigin;
		uint16_t width;
		uint16_t height;
		uint8_t  pixelDepth;
		uint8_t  imageDescriptor;
	};
}

#endif /* header_hpp */
