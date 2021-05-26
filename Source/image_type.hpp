#ifndef image_type_hpp
#define image_type_hpp

namespace tga
{
	enum ImageType
	{
		NoImageData,
		UncompressedColorMapped,
		UncompressedTrueColor,
		UncompressedGrayscale,
		RunLengthEncodedColorMapped,
		RunLengthEncodedTrueColor,
		RunLengthEncodedGrayscale,
	};
}

#endif /* image_type_hpp */
