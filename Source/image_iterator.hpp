#ifndef image_iterator_hpp
#define image_iterator_hpp

#include "header.hpp"
#include "image.hpp"

namespace tga
{
	class ImageIterator
	{
	public:
		ImageIterator();
		ImageIterator(const Header& header, Image& image);
		// TODO: Consider constructor with const image for encoding.

		// Put one pixel value into the image and advance the iterator.
		template<typename T>
		bool putPixel(const T value)
		{
			// TODO: Clean up type cast.
			*((T*)m_ptr) = value;
			//*static_cast<T*>(m_ptr) = value;
			return advance();
		}

		// Get one pixel value from the image and advance the iterator.
		template<typename T>
		T getPixel()
		{
			// TODO: Clean up type cast.
			T value = *((T*)m_ptr);
			//T value = *static_cast<T*>(m_ptr);
			advance();
			return value;
		}

	private:
		bool advance();
		void calcPtr();

		Image* m_image;
		int m_x, m_y;
		int m_w, m_h;
		int m_dx, m_dy;
		uint8_t* m_ptr;
		//unsigned int * m_ptr;
	};
}

#endif /* image_iterator_hpp */
