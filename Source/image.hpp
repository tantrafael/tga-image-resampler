#ifndef image_hpp
#define image_hpp

#include <string>

#include "image_type.hpp"
#include "color_map.hpp"

namespace tga
{
	struct Image
	{
		// Header
		int idLength;
		int colorMapType;
		ImageType imageType;
		int colorMapOrigin;
		int colorMapLength;
		int colorMapBitDepth;
		int originX;
		int originY;
		int width;
		int height;
		int pixelBitDepth;
		int imageDescriptor;
		std::string imageId;
		ColorMap colorMap;

		// Body
		int pixelByteDepth;
		int rowStride;
		uint8_t* pixels;

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

		int getPixelByteDepth() const
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

#endif /* image_hpp */
