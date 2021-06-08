#ifndef kernel_sampler_hpp
#define kernel_sampler_hpp

//#include <map>
#include <unordered_map>

#include "kernel_type.hpp"
#include "kernel_direction.hpp"

namespace tga
{
	// TODO: Add virtual destructor.
	class KernelSampler
	{
		// Bicubic:  BicubicSampler; 0.0f, 1.0f
		// Catmull:  BicubicSampler; 0.0f, 0.5f
		// Mitchell: BicubicSampler; 1.0f / 3.0f, 1.0f / 3.0f
		// Cardinal: BicubicSampler; 0.0f, 0.75f
		// BSpline:  BicubicSampler; 1.0f, 0.0f
		// Lanczos:  LanczosSampler; 1.0f,
		// Lanczos2: LanczosSampler; 2.0f
		// Lanczos3: LanczosSampler; 3.0f
		// Lanczos4: LanczosSampler; 4.0f
		// Lanczos5: LanczosSampler; 5.0f

		/*
		std::map<KernelType, Foo> table
		{
			//{ Bicubic, (BicubicSampler, 0.0f, 1.0f) },
			//{ Catmull, (BicubicSampler, 0.0f, 0.5f) },
			//{ Lanczos, (LanczosSampler, 1.0f) },
			//{ Lanczos2, (LanczosSampler, 2.0f) }
		};
		*/

	public:
		//static KernelSampler* create(const KernelType type);
		static std::shared_ptr<KernelSampler> create(const KernelType type);

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
}

#endif /* kernel_sampler_hpp */
