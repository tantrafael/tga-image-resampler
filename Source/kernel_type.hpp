#ifndef kernel_type_hpp
#define kernel_type_hpp

namespace tga
{
	enum KernelType {
		//Unknown,
		Bicubic,
		Catmull,
		Mitchell,
		Cardinal,
		BSpline,
		Lanczos,
		Lanczos2,
		Lanczos3,
		Lanczos4,
		Lanczos5,
	};
}

#endif /* kernel_type_hpp */
