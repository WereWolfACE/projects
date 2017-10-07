#pragma once

//
// —татический класс, содержащий функции работы со значени€ми типа float
// ¬ частности удобные и ошибкоустойчивые функции перевода
// чисел в строковое представление и обратно.
//
class Float {
public:
    static std::string ToString(float value);
	static std::string ToString(double value);

	//
	// ѕолучение строки с изображением числа типа float с экспонентой (в формате %e).
	//
	static std::string ToStringE(float value);

	//
	// ѕеревод изображени€ вещественного числа вида -?\d+\.\d+ в само число.
	// Ёкспонента не предусмотрена, точка, цифры до и после неЄ об€зательны,
	// знак возможен только минус. ќбщее число цифр не должно быть больше 7.
	//
	static float ParseFixed(const std::string& image);
	static float ParseFixed(const char* image);

	static double ParseDouble(const std::string& image);
	static double ParseDouble(const char* image);

	//
	// ѕолучение строки с точным представлением числа типа float.
	// ‘ормат: идентификатор, что это такой формат, затем вещественное
	// число (дл€ проверки и дл€ чтени€ человеком), затем целое число -
	// биты вещественного числа.
	//
	static std::string ToStringExact(float value);

	//
	// ѕолучение числа типа float из его изображени€, полученного ToStringExact.
	//
	static float ParseExact(const std::string& image);

	//
	// ѕолучение числа типа float из его изображени€, на выбор:
	// или из записи с фиксированной точкой (используетс€ ParseFixed),
	// или из записи с битами (используетс€ ParseExact).
	//
	static float Parse(const std::string& image);

	//
	// ѕолучение строки с изображением числа типа float
	// в формате с фиксированной точкой.
	//  оличество цифр после точки - максимальное дл€ ParseFixed.
	//
	// ќграничени€ на число такие, чтобы ParseFixed смогла его прочитать -
	// ноль или по модулю от 0.000001 до 999999.9.
	//
	static std::string ToStringF(float value);
};
