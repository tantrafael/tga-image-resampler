#include "kernel_sampler.hpp"

#include <memory>

#include "kernel_type.hpp"
#include "bicubic.hpp"
#include "lanczos.hpp"

namespace tga
{
	typedef std::shared_ptr<KernelSampler> sampler_ptr;

	sampler_ptr KernelSampler::create(const KernelType kernelType)
	{
		std::unordered_map<KernelType, sampler_ptr> samplerTable
		{
			{ Bicubic,  sampler_ptr(new BicubicSampler{ 0.0f, 1.0f }) },
			{ Catmull,  sampler_ptr(new BicubicSampler{ 0.0f, 0.5f }) },
			{ Mitchell, sampler_ptr(new BicubicSampler{ 1.0f / 3.0f, 1.0f / 3.0f }) },
			{ Cardinal, sampler_ptr(new BicubicSampler{ 0.0f, 0.75f }) },
			{ BSpline,  sampler_ptr(new BicubicSampler{ 1.0f, 0.0f }) },
			{ Lanczos,  sampler_ptr(new LanczosSampler{ 1.0f }) },
			{ Lanczos2, sampler_ptr(new LanczosSampler{ 2.0f }) },
			{ Lanczos3, sampler_ptr(new LanczosSampler{ 3.0f }) },
			{ Lanczos4, sampler_ptr(new LanczosSampler{ 4.0f }) },
			{ Lanczos5, sampler_ptr(new LanczosSampler{ 5.0f }) },
		};

		sampler_ptr sampler = samplerTable[kernelType];

		return sampler;
	}

	KernelSampler::~KernelSampler()
	{}
}
