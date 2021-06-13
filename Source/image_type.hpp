#ifndef image_type_hpp
#define image_type_hpp

namespace tga
{
	enum ImageType
	{
		NoImageData = 0,
		UncompressedColorMapped = 1,
		UncompressedTrueColor = 2,
		UncompressedGrayscale = 3,
		RunLengthEncodedColorMapped = 9,
		RunLengthEncodedTrueColor = 10,
		RunLengthEncodedGrayscale = 11,
	};
}

#endif /* image_type_hpp */
