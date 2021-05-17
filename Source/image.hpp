#ifndef image_hpp
#define image_hpp

namespace tga
{
	struct Image
	{
		uint8_t* pixels;
		uint32_t pixelByteDepth;
		uint32_t rowStride;
	};
}

#endif /* image_hpp */
