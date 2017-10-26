#ifndef __BINARYREADER_H__
#define __BINARYREADER_H__

#if _MSC_VER > 1200
#pragma once
#endif // _MSC_VER

#include <system_error>

namespace IO {

class InputStream;

class BinaryReader {
public:
	explicit BinaryReader(InputStream* stream);

	InputStream* GetBaseStream() const { return _stream; }

	int8_t ReadInt8();
	int8_t ReadInt8(std::error_code& ec);
	int16_t ReadInt16();
	int16_t ReadInt16(std::error_code& ec);
	int32_t ReadInt32();
	int32_t ReadInt32(std::error_code& ec);
	int32_t Read7BitEncodedInt32();
	int32_t Read7BitEncodedInt32(std::error_code& ec);
	int64_t ReadInt64();
	int64_t ReadInt64(std::error_code& ec);
	uint8_t ReadUInt8() { return static_cast<uint8_t>(ReadInt8()); }
	uint8_t ReadUInt8(std::error_code& ec) { return static_cast<uint8_t>(ReadInt8(ec)); }
	uint16_t ReadUInt16() { return static_cast<uint16_t>(ReadInt16()); }
	uint16_t ReadUInt16(std::error_code& ec) { return static_cast<uint16_t>(ReadInt16(ec)); }
	uint32_t ReadUInt32() { return static_cast<uint32_t>(ReadInt32()); }
	uint32_t ReadUInt32(std::error_code& ec) { return static_cast<uint32_t>(ReadInt32(ec)); }
	uint64_t ReadUInt64() { return static_cast<uint64_t>(ReadInt64()); }
	uint64_t ReadUInt64(std::error_code& ec) { return static_cast<uint64_t>(ReadInt64(ec)); }
	bool ReadBool() { return static_cast<bool>(ReadUInt8()); }
	bool ReadBool(std::error_code& ec) { return static_cast<bool>(ReadUInt8(ec)); }
	float ReadFloat();
	float ReadFloat(std::error_code& ec);
	double ReadDouble();
	double ReadDouble(std::error_code& ec);
	std::string ReadUtf8String();
	std::string ReadUtf8String(std::error_code& ec);

private:
	InputStream* _stream;
};

} // namespace IO

#endif // __BINARYREADER_H__
