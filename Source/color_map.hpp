#ifndef color_map_hpp
#define color_map_hpp

#include <vector>

namespace tga
{
	class ColorMap
	{
	public:
		ColorMap() {}
		ColorMap(const int size) : m_color(size) {}

	private:
		//std::vector<uint_32_t> m_color;
		std::vector<long int> m_color;
	};
}

#endif /* color_map_hpp */
