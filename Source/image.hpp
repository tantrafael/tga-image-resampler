#ifndef image_hpp
#define image_hpp

#include "image_header.hpp"
#include "image_body.hpp"

namespace tga
{
	struct Image
	{
		ImageHeader header;
		ImageBody body;
	};
}

#endif /* image_hpp */
