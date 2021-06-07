#include "kernel_sampler.hpp"

#include <memory>

#include "kernel_type.hpp"
#include "bicubic.hpp"
#include "lanczos.hpp"

namespace tga
{
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

		//KernelSampler* bicubicSampler{ new BicubicSampler{ 0.0f, 1.0f } };
		std::shared_ptr<KernelSampler> bicubicSampler(new BicubicSampler{ 0.0f, 1.0f });
		//table.insert(std::map<KernelType, std::unique_ptr<KernelSampler>>::value_type(Bicubic, bicubicSampler));
		table[Bicubic] = bicubicSampler;

		//std::unique_ptr<KernelSampler> sampler = table[type];
		std::shared_ptr<KernelSampler> sampler = table[type];

		//return nullptr;
		return sampler;
	}
}
