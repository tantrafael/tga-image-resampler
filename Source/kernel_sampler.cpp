#include "kernel_sampler.hpp"

#include <memory>

#include "kernel_type.hpp"
#include "bicubic.hpp"
#include "lanczos.hpp"

namespace tga
{
	//#define VALUE_TYPE std::map<KernelType, std::shared_ptr<KernelSampler>>::value_type

	typedef std::shared_ptr<KernelSampler> sampler_ptr;

	//KernelSampler* KernelSampler::create(const KernelType type)
	//std::shared_ptr<KernelSampler> KernelSampler::create(const KernelType type)
	sampler_ptr KernelSampler::create(const KernelType type)
	{
		/*
		std::map<KernelType, std::function<KernelSampler*()>> table
		{
			{ Bicubic, []() { return new BicubicSampler{ 0.0f, 1.0f }; } },
			{ Lanczos, []() { return new LanczosSampler{ 1.0f }; } }
		};

		std::function<KernelSampler*()> lambda = table[type];
		KernelSampler* sampler = lambda();
		*/

		//std::map<KernelType, KernelSampler*> table{};
		//std::map<KernelType, std::shared_ptr<KernelSampler>> table{};

		/*
		//std::shared_ptr<KernelSampler> bicubicSampler(new BicubicSampler{ 0.0f, 1.0f });
		bar bicubicSampler(new BicubicSampler{ 0.0f, 1.0f });
		bar catmullSampler(new BicubicSampler{ 0.0f, 0.5f });
		bar mitchellSampler(new BicubicSampler{ 1.0f / 3.0f, 1.0f / 3.0f });
		bar cardinalSampler(new BicubicSampler{ 0.0f, 0.75f });
		bar bsplineSampler(new BicubicSampler{ 1.0f, 0.0f });
		bar lanczosSampler(new LanczosSampler{ 1.0f });
		bar lanczos2Sampler(new LanczosSampler{ 2.0f });
		bar lanczos3Sampler(new LanczosSampler{ 3.0f });
		bar lanczos4Sampler(new LanczosSampler{ 4.0f });
		bar lanczos5Sampler(new LanczosSampler{ 5.0f });
		*/

		/*
		//table.insert(std::map<KernelType, std::shared_ptr<KernelSampler>>::value_type(Bicubic, bicubicSampler));
		table.insert(VALUE_TYPE(Bicubic, bicubicSampler));
		table.insert(VALUE_TYPE(Catmull, catmullSampler));
		table.insert(VALUE_TYPE(Mitchell, mitchellSampler));
		table.insert(VALUE_TYPE(Cardinal, cardinalSampler));
		table.insert(VALUE_TYPE(BSpline, bsplineSampler));
		table.insert(VALUE_TYPE(Lanczos, lanczosSampler));
		table.insert(VALUE_TYPE(Lanczos2, lanczos2Sampler));
		table.insert(VALUE_TYPE(Lanczos3, lanczos3Sampler));
		table.insert(VALUE_TYPE(Lanczos4, lanczos4Sampler));
		table.insert(VALUE_TYPE(Lanczos5, lanczos5Sampler));
		*/

		//table[Bicubic] = bar(new BicubicSampler{ 0.0f, 1.0f });

		//std::map<KernelType, sampler_ptr> kernelSamplerTable
		std::unordered_map<KernelType, sampler_ptr> kernelSamplerTable
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

		//std::unique_ptr<KernelSampler> sampler = table[type];
		sampler_ptr sampler = kernelSamplerTable[type];

		//return nullptr;
		return sampler;
	}
}
