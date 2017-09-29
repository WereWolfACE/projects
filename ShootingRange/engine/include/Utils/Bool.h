#ifndef _UTILS_BOOL_H_
#define _UTILS_BOOL_H_

#pragma once

//
// Статический класс, содержащий функции работы со значениями типа bool
// В частности функции перевода в строковое представление и обратно
//
class Bool
{
public:

	//
	// Чтение значения типа bool из строки
	//
	static bool Parse(const std::string& image) {
		if (image == "true") {
			return true;
		} else if (image == "false") {
			return false;
		} else {
			Assert(false); // неожиданное изображение значения типа bool
			return false;
		}
	}

	//
	// Чтение 0 или 1 из строки, выдача false или true соответственно
	//
	static bool Parse01(const std::string& image) {
		if (image == "0") {
			return false;
		} else if (image == "1") {
			return true;
		} else {
			Assert(false); // неожиданное значение
			return false;
		}
	}

	//
	// Получение строки с изображением значения типа bool
	//
	static std::string ToString(bool value) {
		if (value) {
			return "true";
		} else {
			return "false";
		}
	}
};

#endif //_UTILS_BOOL_H_
