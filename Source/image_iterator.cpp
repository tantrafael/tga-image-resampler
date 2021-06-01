#include "image_iterator.hpp"

namespace tga
{
	ImageIterator::ImageIterator()
		: m_image{ nullptr }
	{}

	ImageIterator::ImageIterator(const Header& header, Image& image)
		: m_image{ &image }
		, m_x{ header.isLeftToRight() ? 0 : header.width - 1 }
		, m_y{ header.isTopToBottom() ? 0 : header.height - 1 }
		, m_w{ header.width }
		, m_h{ header.height }
		, m_dx{ header.isLeftToRight() ? 1 : -1 }
		, m_dy{ header.isTopToBottom() ? 1 : -1 }
	{
		calcPtr();
	}

	bool ImageIterator::advance()
	{
		m_x += m_dx;
		m_ptr += m_dx * m_image->pixelByteDepth;

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
		m_ptr = m_image->pixels
				+ m_image->rowStride * m_y
				+ m_image->pixelByteDepth * m_x;
	}
}
