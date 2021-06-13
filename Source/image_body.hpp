#ifndef image_body_hpp
#define image_body_hpp

namespace tga
{
	struct ImageBody
	{
		uint8_t* pixels;
		int pixelByteDepth;
		int rowStride;
	};
}

#endif /* image_body_hpp */
