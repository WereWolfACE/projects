#ifndef __UTILS_INT_H__
#define __UTILS_INT_H__

#pragma once

#include <stdint.h>

/// Статический класс, содержащий функции работы со значениями типа int
/// В частности удобные и ошибкоустойчивые функции перевода
/// чисел в строковое представление и обратно.
class Int
{
public:

	/// максимальное значение типа int
	static const int MAX_VALUE = 2147483647;

	/// минимальное значение типа int
	static const int MIN_VALUE = -2147483647 - 1;

	/// строка с десятичной записью максимального значения типа int
	static const std::string MAX_VALUE_IMAGE;

	/// строка с десятичной записью минимального значения типа int
	static const std::string MIN_VALUE_IMAGE;

	/// Чтение целого числа со знаком из его десятичной записи.
	static int Parse(const std::string& image);
	static int Parse(const char* image);

	static long ParseLong(const std::string& image);
	static long ParseLong(const char* image);

	static long long ParseLongLong(const std::string& image);
	static long long ParseLongLong(const char* image);

	static uint32_t ParseUint(const std::string& image);
	static uint32_t ParseUint(const char* image);

	/// Чтение целого числа со знаком из его восьмеричной записи.
	/// Пробелы и ведущие нули допускаются.
	static int Parse8(const std::string& image);
	static int64_t Parse8_64(const std::string& image);

	/// Чтение целого числа из его шестнадцатеричной записи.
	static int ParseHex(const std::string& image);

	/// Получение строки с изображением числа типа int.
	static std::string ToString(int value);

	/// Получение строки с изображением числа типа int64.
	static std::string ToString(int64_t value);

	/// Перевод шестнадцатеричной цифры в символ с её изображением.
	static char ToHexChar(int value);

	/// Получение строки с шестнадцатеричной записью числа типа int.
	/// Только шестнадцатеричные цифры, никакого префикса/постфикса нет.
	static std::string ToHexString(int value);

	/// Получение длины десятичной записи числа типа int.
	static int Length(int value);
};

#endif // __UTILS_INT_H__
