#ifndef __STREAM_H__
#define __STREAM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "RefCounter.h"

#include <vector>

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/system/system_error.hpp>

#define DISALLOW_COPY_AND_ASSIGN(Class) \
	Class(const Class&) = delete; \
	Class& operator=(const Class&) = delete;

namespace IO {

enum class Origin {
	Beginning,
	Current,
	End
};

class OutputStream;

class Seekable : public RefCounter {
public:
	Seekable() = default;

	virtual void Seek(int offset, Origin origin, boost::system::error_code& ec) = 0;

	virtual size_t Tell(boost::system::error_code& ec) const = 0;

	bool Seek(int offset, Origin origin) {
		boost::system::error_code ec;
		Seek(offset, origin, ec);
		return !ec;
	}

	size_t Tell() {
		boost::system::error_code ec;
		auto ret = Tell(ec);
		return !ec ? ret : -1;
	}

private:
	DISALLOW_COPY_AND_ASSIGN(Seekable)
};


class InputStream : virtual public Seekable {
public:
	virtual size_t Read(void* buffer, size_t length, boost::system::error_code& ec) = 0;

	size_t Read(void* buffer, size_t length) {
		boost::system::error_code ec;
		return Read(buffer, length, ec);
	}

	void ReadAllBytes(std::vector<uint8_t>& data, boost::system::error_code& ec);

	void ReadAllBytes(OutputStream* stream, boost::system::error_code& ec);

	bool ReadAllBytes(std::vector<uint8_t>& data) {
		boost::system::error_code ec;
		ReadAllBytes(data, ec);
		return !ec;
	}

	bool ReadAllBytes(OutputStream* stream) {
		boost::system::error_code ec;
		ReadAllBytes(stream, ec);
		return !ec;
	}

	using Seekable::Seek;
	using Seekable::Tell;
};

typedef boost::intrusive_ptr<InputStream> InputStreamPtr;


class OutputStream : virtual public Seekable {
public:
	virtual size_t Write(const void* buffer, size_t length, boost::system::error_code& ec) = 0;

	virtual void Flush(boost::system::error_code& ec) = 0;

	void WriteAllBytes(const std::vector<uint8_t>& data, boost::system::error_code& ec);

	void WriteAllBytes(InputStream* stream, boost::system::error_code& ec);

	size_t Write(const void* buffer, size_t length) {
		boost::system::error_code ec;
		return Write(buffer, length, ec);
	}

	bool Flush() {
		boost::system::error_code ec;
		Flush(ec);
		return !ec;
	}

	bool WriteAllBytes(const std::vector<uint8_t>& data) {
		boost::system::error_code ec;
		WriteAllBytes(data, ec);
		return !ec;
	}

	bool WriteAllBytes(InputStream* stream) {
		boost::system::error_code ec;
		WriteAllBytes(stream, ec);
		return !ec;
	}

	using Seekable::Seek;
	using Seekable::Tell;
};

typedef boost::intrusive_ptr<OutputStream> OutputStreamPtr;


class Stream : public InputStream, public OutputStream {
public:
	using Seekable::Seek;
	using Seekable::Tell;
	using InputStream::Read;
	using OutputStream::Write;
	using OutputStream::Flush;
};

typedef boost::intrusive_ptr<Stream> StreamPtr;

class InputStreamSource {
public:
	typedef char char_type;
	typedef struct seekable_source_tag : boost::iostreams::device_tag, boost::iostreams::input_seekable { } category;

	explicit InputStreamSource(const InputStreamPtr& stream)
		: _stream(stream)
	{
	}

	std::streamsize read(char_type* s, std::streamsize n) {
		return static_cast<std::streamsize>(_stream->Read(s, static_cast<size_t>(n)));
	}

	std::streampos seek(boost::iostreams::stream_offset off, std::ios_base::seekdir way) {
		Origin origin;

		switch (way) {
		default:
			Halt("Unknown value");
			// go through
		case std::ios_base::beg:
			origin = Origin::Beginning;
			break;
		case std::ios_base::cur:
			origin = Origin::Current;
			break;
		case std::ios_base::end:
			origin = Origin::End;
			break;
		};

		boost::system::error_code ec;
		_stream->Seek(static_cast<int>(off), origin, ec);
		Assert2(!ec, ec.message());
		
		return static_cast<std::streampos>(_stream->Tell());
	}

private:
	InputStreamPtr _stream;

private:
	InputStreamSource& operator=(const InputStreamSource&) = delete;
};

class OutputStreamSink {
public:
	typedef char char_type;
	typedef boost::iostreams::sink_tag category;

	explicit OutputStreamSink(const OutputStreamPtr& stream)
		: _stream(stream)
	{
	}

	std::streamsize write(const char_type* s, std::streamsize n) {
		return static_cast<std::streamsize>(_stream->Write(s, static_cast<size_t>(n)));
	}

private:
	OutputStreamPtr _stream;

private:
	OutputStreamSink& operator=(const OutputStreamSink&) = delete;
};

typedef boost::iostreams::stream<InputStreamSource> ISStream;
typedef boost::iostreams::stream<OutputStreamSink> OSStream;

} // namespace IO

#endif // __STREAM_H__
