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
					  Image& targetImage,
					  const unsigned int bufferSize);

		
		/*
		bool resample(const Header& sourceHeader,
					  const Image& sourceImage,
					  const uint8_t width,
					  const uint8_t height,
					  Header& targetHeader,
					  Image& targetImage,
					  const uint8_t bufferSize);
		*/
	};
}

#endif /* resampler_hpp */
