#ifndef _UTF8STRING_H_
#define _UTF8STRING_H_

//
// Класс строки в кодировке Utf8.
// Содержит итератор, с помощью которого можно "ходить"
// по строке, не задумываясь о ее внутреннем устройстве.
// Например, так:
//
// for (Utf8String::Iterator i = s.Begin(); i != s.End(); ++i) {
//     std::string letter = *i;
//	   // делаем что-то с символом letter
// }
//
class Utf8String {

public:

	class Iterator;

	// Конструктор из стандартной строки
	explicit Utf8String(const std::string& str);

	// Возвращает итератор начала строки (первого символа)
	Iterator Begin() const;

	// Возвращает итератор конца строки (за последним символом)
	Iterator End() const;

	// Возвращает подстроку [begin, end)
	std::string GetSubstring(Iterator begin, Iterator end) const;

private:
	
	//friend class Iterator; -- вложенный класс автоматически становится другом.

	std::string _string;
		// исходная строка

};

// Итератор по строке
class Utf8String::Iterator {
public:

	// Префиксный инкремент
	Iterator& operator++();

	// Постфиксный инкремент
	Iterator operator++(int);

	// Получение символа строки
	std::string operator *() const;

	// Сравнение итераторов
	bool operator != (const Iterator& other) const;

	friend class Utf8String;

private:


	size_t _letterPos;
		// позиция в строке текущего символа
		// итератор считается конечным, если _letterPos == std::string::npos

	const Utf8String& _string;
		// ссылка на итерируемый объект

	// Конструктор
	Iterator(const Utf8String& str, size_t pos);

	// Вернуть позицию следующего символа
	// или std::string::npos, если такого нет
	size_t GetNextPos() const;

}; // class Iterator

#endif
