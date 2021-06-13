#include "stdio_file_interface.hpp"

#include <unordered_map>
#include <cassert>
#include <limits>

namespace tga
{
	StdioFileInterface::StdioFileInterface(const std::string filename,
										   const FileAccessMode mode)
		: FileInterface(filename, mode)
	{
		std::unordered_map<FileAccessMode, std::string> modeStringTable
		{
			{ ReadBinary,  "rb" },
			{ WriteBinary, "wb" }
		};

		const auto modeString{ modeStringTable[mode] };

		m_file = std::fopen(filename.data(), modeString.data());
		m_ok = true;
	}

	bool StdioFileInterface::ok() const
	{
		return m_ok;
	}

	size_t StdioFileInterface::tell()
	{
		return ftell(m_file);
	}

	void StdioFileInterface::seek(size_t absPos)
	{
		// To detect surprises with the size_t -> long cast.
		assert(absPos <= std::numeric_limits<long>::max());

		fseek(m_file, static_cast<long>(absPos), SEEK_SET);
	}

	//uint8_t StdioFileInterface::read8()
	int StdioFileInterface::read8()
	{
		const int value{ fgetc(m_file) };

		if (value != EOF) {
			// We can safely cast to uint8_t, as EOF is the only special
			// non-uint8 value that fgetc() should return.
			//return static_cast<uint8_t>(value);
			return value;
		}
		else
		{
			m_ok = false;
			return 0;
		}
	}

	//void StdioFileInterface::write8(uint8_t value)
	void StdioFileInterface::write8(const int value)
	{
		fputc(value, m_file);
	}

	void StdioFileInterface::close()
	{
		std::fclose(m_file);
		m_ok = false;
	}
}
