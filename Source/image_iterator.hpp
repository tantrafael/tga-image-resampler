#ifndef image_iterator_hpp
#define image_iterator_hpp

#include "image.hpp"

namespace tga
{
	class ImageIterator
	{
	public:
		ImageIterator();
		ImageIterator(Image& image);

		// Put one pixel value into the image and advance the iterator.
		template<typename T>
		void putPixel(const T value)
		{
			*reinterpret_cast<T*>(m_ptr) = value;
			advance();
		}

		// Get one pixel value from the image and advance the iterator.
		template<typename T>
		T getPixel()
		{
			T value = *reinterpret_cast<T*>(m_ptr);
			advance();
			return value;
		}

	private:
		bool advance();
		void calcPtr();

		//ImageBody* m_body;
		Image* m_image;
		int m_x, m_y;
		int m_w, m_h;
		int m_dx, m_dy;
		uint8_t* m_ptr;
	};
}

#endif /* image_iterator_hpp */
