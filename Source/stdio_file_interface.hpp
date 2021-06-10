#ifndef stdio_file_interface_hpp
#define stdio_file_interface_hpp

#include <cstdio>

#include "file_interface.hpp"

namespace tga
{
	class StdioFileInterface : public tga::FileInterface
	{
	public:
		StdioFileInterface(const std::string filename, const FileAccessMode mode);
		bool ok() const override;
		size_t tell() override;
		void seek(size_t absPos) override;
		uint8_t read8() override;
		void write8(uint8_t value) override;
		void close() override;

	private:
		FILE* m_file;
		bool m_ok;
	};
}

#endif /* stdio_file_interface_hpp */
