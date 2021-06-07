#include "kernel_sampler.hpp"

#include "kernel_type.hpp"
#include "bicubic.hpp"
#include "lanczos.hpp"

namespace tga
{
	KernelSampler* KernelSampler::create(const KernelType type)
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
}
