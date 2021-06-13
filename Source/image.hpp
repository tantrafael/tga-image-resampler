#ifndef image_hpp
#define image_hpp

#import "image_header.hpp"
#import "image_body.hpp"

namespace tga
{
	struct Image
	{
		ImageHeader header;
		ImageBody body;
	};
}

#endif /* image_hpp */
