#ifndef header_hpp
#define header_hpp

#include <string>

#include "image_type.hpp"
#include "color_map.hpp"

namespace tga
{
	struct Header
	{
		uint8_t idLength;
		uint8_t colorMapType;
		uint8_t imageType;
		uint16_t colorMapOrigin;
		uint16_t colorMapLength;
		uint8_t colorMapDepth;
		uint16_t xOrigin;
		uint16_t yOrigin;
		uint16_t width;
		uint16_t height;
		uint8_t pixelDepth;
		uint8_t imageDescriptor;
		std::string imageId;
		ColorMap colorMap;

		bool leftToRight() const
		{
			return !(imageDescriptor & 0x10);
		}

		bool topToBottom() const
		{
			return !(imageDescriptor & 0x20);
		}

		bool isTrueColor() const
		{
			return
			(
				imageType == UncompressedTrueColor
				|| imageType == RunLengthEncodedTrueColor
			);
		}

		int pixelByteDepth() const
		{
			if (isTrueColor())
			{
				return 4;
			}
			else
			{
				return 5;
			}
		}
	};
}

#endif /* header_hpp */
