#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "IO/Stream.h"

namespace IO {

enum class AccessMode {
	ReadOnly,
	ReadWrite
};

enum class CreateMode {
	OpenExisting,
	OpenOrCreate,
	CreateAlways
};

class FileStream : public Stream {
public:
	FileStream();

	FileStream(const std::string& filename, AccessMode accessMode = AccessMode::ReadOnly, CreateMode createMode = CreateMode::OpenExisting);

	~FileStream();

	void Open(const std::string& filename, AccessMode accessMode, CreateMode createMode, boost::system::error_code& ec);

	virtual void Seek(int offset, Origin origin, boost::system::error_code& ec) override;

	virtual size_t Tell(boost::system::error_code& ec) const override;

	virtual size_t Read(void* buffer, size_t length, boost::system::error_code& ec) override;

	virtual size_t Write(const void* buffer, size_t length, boost::system::error_code& ec) override;

	virtual void Flush(boost::system::error_code& ec) override;

	bool IsValid() const;

	using Seekable::Seek;
	using Seekable::Tell;
	using InputStream::Read;
	using OutputStream::Write;
	using OutputStream::Flush;

private:
	std::string _filename;

	FILE* _fp;
};

typedef boost::intrusive_ptr<FileStream> FileStreamPtr;

} // namespace IO

#endif // __FILESTREAM_H__
