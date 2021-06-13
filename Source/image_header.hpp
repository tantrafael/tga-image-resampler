#ifndef image_header_hpp
#define image_header_hpp

#include <string>

#include "image_type.hpp"
#include "color_map.hpp"

namespace tga
{
	struct ImageHeader
	{
		uint8_t idLength;
		uint8_t colorMapType;
		ImageType imageType;
		uint16_t colorMapOrigin;
		uint16_t colorMapLength;
		uint8_t colorMapBitDepth;
		uint16_t originX;
		uint16_t originY;
		//uint16_t width;
		//uint16_t height;
		int width;
		int height;
		uint8_t pixelBitDepth;
		uint8_t imageDescriptor;
		std::string imageId;
		ColorMap colorMap;

		bool isLeftToRight() const
		{
			return !(imageDescriptor & 0x10);
		}

		bool isTopToBottom() const
		{
			return !(imageDescriptor & 0x20);
		}

		bool isTrueColor() const
		{
			return (imageType == UncompressedTrueColor
					|| imageType == RunLengthEncodedTrueColor);
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

		bool validate() const
		{
			switch (imageType)
			{
				case NoImageData:
					return false;
				case UncompressedColorMapped:
				case RunLengthEncodedColorMapped:
					return (pixelBitDepth == 8);
				case UncompressedTrueColor:
				case RunLengthEncodedTrueColor:
					return (pixelBitDepth == 15
							|| pixelBitDepth == 16
							|| pixelBitDepth == 24
							|| pixelBitDepth == 32);
				case UncompressedGrayscale:
				case RunLengthEncodedGrayscale:
					return (pixelBitDepth == 8);
			}

			return false;
		}
	};
}

#endif /* image_header_hpp */
