#ifndef __UTILS_UTILS_H__
#define __UTILS_UTILS_H__

#pragma once

#include "platform.h"
#include "Utils/IPoint.h"
#include "Utils/Color.h"
#include "Utils/Float.hpp"
#include "Utils/Int.h"
#include "EngineAssert.h"
#include "ThreadSupport.h"

#define DECLARE_ENUM_CLASS_FLAGS(Enum) \
	constexpr Enum operator|(Enum lhs, Enum rhs) noexcept { return (Enum)((__underlying_type(Enum))lhs | (__underlying_type(Enum))rhs); } \
	constexpr Enum operator&(Enum lhs, Enum rhs) noexcept { return (Enum)((__underlying_type(Enum))lhs & (__underlying_type(Enum))rhs); } \
	constexpr Enum operator~(Enum e) noexcept { return (Enum)~(__underlying_type(Enum))e; } \
	constexpr bool operator!(Enum e) noexcept { return !(__underlying_type(Enum))e; }

enum class CompressMode {
	Zip,
	GZip
};

namespace utils {

/// Возвращает описание ошибки
std::string error_code_message(const boost::system::error_code& ec);

// Кодирует массив байтов в base64 строку
std::string encode64(const std::vector<uint8_t>& data);
// Декодирует массив байтов из base64 строки
std::vector<uint8_t> decode64(const std::string& base64);

// Сжимает данные в zip/gzip поток
bool deflate(CompressMode mode, const void* uncompressedData, size_t length, std::vector<uint8_t>& compressedData);
// Распаковывает данные из zip/gzip потока
bool inflate(CompressMode mode, const void* compressedData, size_t length, std::vector<uint8_t>& uncompressedData);

FILE* fopen(const std::string& filename, const std::string& mode);
FILE* fopen(const std::wstring& filename, const std::wstring& mode);

// это служит для открытия сайтов в системном браузере а не то что вы подумали
// для iOS тело определёно в Platform/iphone.mm
void OpenPath(const std::string& sitePath);
/// Мультиплатформенный sleep, время в миллисекундах (в тысячных долях)
void Sleep(uint32_t msec);

//
// Вероятно, переводится как "Read `name=value` pair";
// парсит строку формата `name=value`, записывает имя в name и значение в value.
// Возвращает true, если строка в нужном формате и false в противном случае.
//
bool ReadNvp(const std::string& str, std::string& name, std::string& value);

template <class Coll>
inline void tokenize(const std::string& str, Coll &token, const std::string& separators = std::string(" /\t:,()"))
{
	std::string input(str);
	std::string::size_type idx = 0;
		
	if ((idx = input.find_first_not_of(separators)) == std::string::npos) {
		return;
	}
		
	input = input.substr(idx);
	while ((idx = input.find_first_of(separators)) != std::string::npos) {
		token.push_back(input.substr(0, idx));
		input = input.substr(idx + 1);
		idx = input.find_first_not_of(separators);
		if (idx == std::string::npos) {
			break;
		}
		input = input.substr(idx);
	}
		
	if ((input.find_first_of(separators)) == std::string::npos) {
		token.push_back(input);
	}
}

bool equals(const char* a, const char* b);
bool iequals(const char* a, const char* b);

template <class T>
inline T lexical_cast(const char* str);

template <class T>
inline T lexical_cast(const std::string& str) { return lexical_cast<T>(str.c_str()); }

template <>
inline float lexical_cast(const char* str)
{
	Assert(str != NULL);
	return Float::ParseFixed(str);
}
	
template <>
inline double lexical_cast(const char* str)
{
	Assert(str != NULL);
	return Float::ParseDouble(str);
}
	
template <>
inline short lexical_cast(const char* str)
{
	Assert(str != NULL);
	return (short)Int::Parse(str);
}
	
template <>
inline int lexical_cast(const char* str)
{
	Assert(str != NULL);
	return Int::Parse(str);
}
	
template <>
inline unsigned int lexical_cast(const char* str)
{
	Assert(str != NULL);
	return Int::ParseUint(str);
}
	
template <>
inline long lexical_cast(const char* str)
{
	Assert(str != NULL);
	return Int::ParseLong(str);
}

template <>
inline unsigned long lexical_cast(const char* str)
{
    Assert(str != NULL);
    return Int::ParseLong(str);
}

template <>
inline long long lexical_cast(const char* str)
{
	return Int::ParseLongLong(str);
}

template <>
inline unsigned char lexical_cast(const char* str)
{
	Assert(str != NULL);
	return (unsigned char)Int::Parse(str);
}

template <>
inline bool lexical_cast(const char* str)
{
	Assert(str != NULL);
	if (iequals(str, "true") || iequals(str, "yes") || equals(str, "1")) {
		return true;
	}
	return false;
}

template <>
inline std::string lexical_cast(const char* value) 
{
	return value;
}

template <>
inline Color lexical_cast(const char* str)
{
	return Color(str);
}

template <>
inline Color lexical_cast(const std::string& str)
{
	return Color(str);
}

inline std::string lexical_cast(const std::string& value) {
	return value;
}
	
inline std::string lexical_cast(float value, int precision = 16)
{
	std::ostringstream ss;
	ss << std::setprecision(precision) << value;
	return ss.str();
}

inline std::string lexical_cast(double value, int precision = 16)
{
	std::ostringstream ss;
	ss << std::setprecision(precision) << value;
	return ss.str();
}

inline std::string lexical_cast(char value)
{
	std::ostringstream ss;
	ss << (int)value;
	return ss.str();
}

inline std::string lexical_cast(unsigned char value)
{
	std::ostringstream ss;
	ss << (int)value;
	return ss.str();
}

inline std::string lexical_cast(Color color)
{
	char buffer[16];
	sprintf_s(buffer, sizeof(buffer), "#%02X%02X%02X%02X", color.red, color.green, color.blue, color.alpha);
	return std::string(buffer);
}
	
template <class T>
inline std::string lexical_cast(const T& value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

/// Выводит в лог список текстур и занимаемую ими память
void DumpTextures();

bool IsMainThread();
void SetCurrentThreadAsMainThread();

}

#endif // __UTILS_UTILS_H__
