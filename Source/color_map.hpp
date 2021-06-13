#ifndef color_map_hpp
#define color_map_hpp

#include <vector>

#include "color.hpp"

namespace tga
{
	class ColorMap
	{
	public:
		ColorMap() {}
		ColorMap(const int size) : m_color(size) {}

	private:
		std::vector<color> m_color;
	};
}

#endif /* color_map_hpp */
