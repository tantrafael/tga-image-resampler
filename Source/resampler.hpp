#ifndef resampler_hpp
#define resampler_hpp

#include "header.hpp"
#include "image.hpp"

namespace tga
{
	class Resampler
	{
	public:
		Resampler();

		bool resample(const Header& sourceHeader,
					  const Image& sourceImage,
					  Header& targetHeader,
					  Image& targetImage);
	};
}

#endif /* resampler_hpp */
