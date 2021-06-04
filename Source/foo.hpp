#ifndef foo_hpp
#define foo_hpp

namespace tga
{
	// TODO: Add virtual destructor.
	class Foo
	{
	public:
		/*
		virtual bool operator() (const float subPixelPosX,
								 const float subPixelPosY,
								 const KernelDirection direction,
								 uint8_t* pixels,
								 const int32_t width,
								 const int32_t height,
								 float& sampleCount,
								 float (&totalSamples)[3]) = 0;
		*/

		virtual bool sample(const float subPixelPosX,
							const float subPixelPosY,
							const KernelDirection direction,
							uint8_t* pixels,
							const int32_t width,
							const int32_t height,
							float& sampleCount,
							float (&totalSamples)[3]) = 0;
	};

	class Factory
	{
	public:
		/*
		static Foo create()
		{
			return ();
		}
		*/
	};
}

#endif /* foo_hpp */
