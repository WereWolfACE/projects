#ifndef __MEMORYSTREAM_H__
#define __MEMORYSTREAM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "IO/Stream.h"

namespace IO {

class MemoryStream : public Stream {
public:
	enum Mode { Mutable, Const };

	explicit MemoryStream(size_t capacity = 0);

	MemoryStream(void* buffer, size_t length, Mode mode = Mutable);

	~MemoryStream();

	Mode GetMode() const { return _mode; }

	uint8_t* GetBuffer() const;

	size_t GetLength() const;

	bool HasFixedLength() const;

	virtual void Seek(int offset, Origin origin, boost::system::error_code& ec) override;

	virtual size_t Tell(boost::system::error_code& ec) const override;

	virtual size_t Read(void* buffer, size_t length, boost::system::error_code& ec) override;

	virtual size_t Write(const void* buffer, size_t length, boost::system::error_code& ec) override;

	virtual void Flush(boost::system::error_code& ec) override;

	using Seekable::Seek;
	using Seekable::Tell;
	using InputStream::Read;
	using OutputStream::Write;
	using OutputStream::Flush;

private:
	typedef std::vector<uint8_t> buffer_type;

	Mode _mode;

	union {
		uint8_t* _buffer;
		buffer_type* _vec;
	};

	size_t _length;
	size_t _position;
};

typedef boost::intrusive_ptr<MemoryStream> MemoryStreamPtr;

} // namespace IO

#endif // __MEMORYSTREAM_H__
