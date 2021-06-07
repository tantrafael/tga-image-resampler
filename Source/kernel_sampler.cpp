#include "kernel_sampler.hpp"

#include <memory>

#include "kernel_type.hpp"
#include "bicubic.hpp"
#include "lanczos.hpp"

namespace tga
{
	#define FOO std::map<KernelType, std::shared_ptr<KernelSampler>>::value_type

	typedef std::shared_ptr<KernelSampler> bar;

	//KernelSampler* KernelSampler::create(const KernelType type)
	std::shared_ptr<KernelSampler> KernelSampler::create(const KernelType type)
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
		std::map<KernelType, std::shared_ptr<KernelSampler>> table{};

		/*
		KernelSampler* bicubicSampler{ new BicubicSampler{ 0.0f, 1.0f } };
		table.insert(std::map<KernelType, KernelSampler*>::value_type(Bicubic, bicubicSampler));
		KernelSampler* sampler = table[type];
		*/

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

		//table.insert(std::map<KernelType, std::shared_ptr<KernelSampler>>::value_type(Bicubic, bicubicSampler));
		table.insert(FOO(Bicubic, bicubicSampler));
		table.insert(FOO(Catmull, catmullSampler));
		table.insert(FOO(Mitchell, mitchellSampler));
		table.insert(FOO(Cardinal, cardinalSampler));
		table.insert(FOO(BSpline, bsplineSampler));
		table.insert(FOO(Lanczos, lanczosSampler));
		table.insert(FOO(Lanczos2, lanczos2Sampler));
		table.insert(FOO(Lanczos3, lanczos3Sampler));
		table.insert(FOO(Lanczos4, lanczos4Sampler));
		table.insert(FOO(Lanczos5, lanczos5Sampler));
		//table[Bicubic] = bicubicSampler;

		//std::unique_ptr<KernelSampler> sampler = table[type];
		std::shared_ptr<KernelSampler> sampler = table[type];

		//return nullptr;
		return sampler;
	}
}
