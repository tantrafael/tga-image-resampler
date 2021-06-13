#ifndef image_body_hpp
#define image_body_hpp

namespace tga
{
	struct ImageBody
	{
		uint8_t* pixels;
		//uint32_t pixelByteDepth;
		//uint32_t rowStride;
		int pixelByteDepth;
		int rowStride;
	};
}

#endif /* image_body_hpp */
