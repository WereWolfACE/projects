#ifndef __TEXTREADER_H__
#define __TEXTREADER_H__

#if _MSC_VER > 1200
#pragma once
#endif // _MSC_VER

namespace IO {

class InputStream;

class TextReader {
public:
	explicit TextReader(InputStream* stream);

	InputStream* GetBaseStream() const { return _stream; }

	std::string ReadAsciiLine();
	std::string ReadUtf8Line();

	bool ReadAsciiLine(std::string& line);
	bool ReadUtf8Line(std::string& line);

	bool ReadAsciiChar(char& ch);
	bool ReadUtf8Char(uint32_t& ch);

private:
	InputStream* _stream;
};

} // namespace IO

#endif // __TEXTREADER_H__
