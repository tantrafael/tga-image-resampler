#include "image_iterator.hpp"

namespace tga
{
	ImageIterator::ImageIterator()
		: m_body{ nullptr }
	{}

	//ImageIterator::ImageIterator(const ImageHeader& header, ImageBody& body)
	ImageIterator::ImageIterator(Image& image)
		: m_body{ &image.body }
		, m_x{ image.header.isLeftToRight() ? 0 : image.header.width - 1 }
		, m_y{ image.header.isTopToBottom() ? 0 : image.header.height - 1 }
		, m_w{ image.header.width }
		, m_h{ image.header.height }
		, m_dx{ image.header.isLeftToRight() ? 1 : -1 }
		, m_dy{ image.header.isTopToBottom() ? 1 : -1 }
	{
		calcPtr();
	}

	bool ImageIterator::advance()
	{
		m_x += m_dx;
		m_ptr += m_dx * m_body->pixelByteDepth;

		if ((m_dx < 0 && m_x < 0) || (m_dx > 0 && m_x == m_w))
		{
			m_x = (m_dx > 0 ? 0 : m_w - 1);
			m_y += m_dy;

			if ((m_dy < 0 && m_y < 0) || (m_dy > 0 && m_y == m_h))
			{
				return false;
			}

			calcPtr();
		}

		return true;
	}

	void ImageIterator::calcPtr()
	{
		m_ptr = m_body->pixels
				+ m_body->rowStride * m_y
				+ m_body->pixelByteDepth * m_x;
	}
}
