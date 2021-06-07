#ifndef kernel_sampler_factory_hpp
#define kernel_sampler_factory_hpp

#include <map>

#include "kernel_type.hpp"
#include "kernel_sampler.hpp"
#include "bicubic.hpp"
#include "lanczos.hpp"

namespace tga
{
	class KernelSamplerFactory
	{
	// Bicubic:  BicubicSampler; 0.0f, 1.0f
	// Catmull:  BicubicSampler; 0.0f, 1.0f
	// Mitchell: BicubicSampler; 0.0f, 1.0f
	// Cardinal: BicubicSampler; 0.0f, 1.0f
	// BSpline:  BicubicSampler; 0.0f, 1.0f
	// Lanczos:  LanczosSampler; 1.0f,
	// Lanczos2: LanczosSampler; 2.0f
	// Lanczos3: LanczosSampler; 3.0f
	// Lanczos4: LanczosSampler; 4.0f
	// Lanczos5: LanczosSampler; 5.0f

	public:
		static KernelSampler* create(const KernelType type)
		{
			std::map<KernelType, std::function<KernelSampler*()>> table
			{
				{ Bicubic, []() { return new BicubicSampler{ 0.0f, 1.0f }; } },
				{ Lanczos, []() { return new LanczosSampler{ 1.0f }; } }
			};

			std::function<KernelSampler*()> lambda = table[type];
			KernelSampler* sampler = lambda();

			return sampler;
		}
	};
}

#endif /* kernel_sampler_factory_hpp */
