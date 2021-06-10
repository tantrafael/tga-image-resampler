#ifndef file_interface_hpp
#define file_interface_hpp

#include <string>
#include <cinttypes>

namespace tga
{
	enum FileAccessMode
	{
		ReadBinary,
		WriteBinary,
	};

	class FileInterface
	{
	public:
		// Opens the file in specified access mode.
		FileInterface(const std::string filename, const FileAccessMode mode) {}

		virtual ~FileInterface() {}

		// Returns true if we can read/write bytes from/into the file.
		virtual bool ok() const = 0;

		// Current position in the file.
		virtual size_t tell() = 0;

		// Jump to the given position in the file.
		virtual void seek(size_t absPos) = 0;

		// Returns the next byte in the file or 0 if ok() = false.
		virtual uint8_t read8() = 0;

		// Writes one byte in the file (or do nothing if ok() = false).
		virtual void write8(uint8_t value) = 0;

		// Closes the file.
		virtual void close() = 0;
	};
}

#endif /* file_interface_hpp */
