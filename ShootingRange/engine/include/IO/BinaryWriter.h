#ifndef __BINARYWRITER_H__
#define __BINARYWRITER_H__

#if _MSC_VER > 1200
#pragma once
#endif // _MSC_VER

#include <system_error>

namespace IO {

class OutputStream;

class BinaryWriter {
public:
	explicit BinaryWriter(OutputStream* stream);

	OutputStream* GetBaseStream() const { return _stream; }

	void WriteInt8(int8_t value, std::error_code& ec);
	void WriteInt8(int8_t value);
	void WriteInt16(int16_t value, std::error_code& ec);
	void WriteInt16(int16_t value);
	void WriteInt32(int32_t value, std::error_code& ec);
	void WriteInt32(int32_t value);
	void Write7BitEncodedInt32(int32_t value, std::error_code& ec);
	void Write7BitEncodedInt32(int32_t value);
	void WriteInt64(int64_t value, std::error_code& ec);
	void WriteInt64(int64_t value);
	void WriteUInt8(uint8_t value, std::error_code& ec) { WriteInt8(static_cast<int8_t>(value), ec); }
	void WriteUInt8(uint8_t value) { WriteInt8(static_cast<int8_t>(value)); }
	void WriteUInt16(uint16_t value, std::error_code& ec) { WriteInt16(static_cast<int16_t>(value), ec); }
	void WriteUInt16(uint16_t value) { WriteInt16(static_cast<int16_t>(value)); }
	void WriteUInt32(uint32_t value, std::error_code& ec) { WriteInt32(static_cast<int32_t>(value), ec); }
	void WriteUInt32(uint32_t value) { WriteInt32(static_cast<int32_t>(value)); }
	void WriteUInt64(uint64_t value, std::error_code& ec) { WriteInt64(static_cast<int64_t>(value), ec); }
	void WriteUInt64(uint64_t value) { WriteInt64(static_cast<int64_t>(value)); }
	void WriteBool(bool value, std::error_code& ec) { WriteUInt8(static_cast<uint8_t>(value), ec); }
	void WriteBool(bool value) { WriteUInt8(static_cast<uint8_t>(value)); }
	void WriteFloat(float value, std::error_code& ec);
	void WriteFloat(float value);
	void WriteDouble(double value, std::error_code& ec);
	void WriteDouble(double value);
	void WriteUtf8String(const std::string& value, std::error_code& ec);
	void WriteUtf8String(const std::string& value);
	void WriteUtf8String(const char* value, std::error_code& ec);
	void WriteUtf8String(const char* value);


private:
	OutputStream* _stream;
};

} // namespace IO

#endif // __BINARYWRITER_H__
