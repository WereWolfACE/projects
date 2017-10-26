#ifndef _UTILS_STR_H_
#define _UTILS_STR_H_

namespace utils {

/// Статический класс, содержащий функции работы со значениями типа std::string
class String
{
public:
	/// Чтение строки из файла до перевода строки (который не возвращается частью строки)
	/// или до конца файла. Если уже конец файла - будет выдана пустая строка
	static std::string ReadLine(FILE *f);

	/// Если head является началом s, то оно отрезается и выдаётся true,
	/// в противном случае с s ничего не происходит и выдаётся false.
	static bool IsHeadCut(std::string &s, const char *head);

	/// Преобразует символы строки к нижнему регистру
	static std::string ToLower(const std::string& str);

	/// Преобразует символы строки к верхнему регистру
	static std::string ToUpper(const std::string& str);

	/// Отсекает от концов строки пробелы (или других заданные символы)
	static std::string Trim(const std::string& str, char ch = ' ');

	/// Разбивает строку на части по указанному разделителю
	static std::vector<std::string> Split(const std::string& str, char ch, bool autotrim = false);

	static std::string Pad(const std::string& str, const std::string& pad, bool not_first_line = false);
    
	/// Заменяет одну подстоку на указанную
    static std::string Replace( const std::string& str, const std::string& from, const std::string& to);
	
	/// Заменяет все подстроки на указанную
	static std::string ReplaceAll( const std::string& str, const std::string& from, const std::string& to);

	/// Проверяет, что строка начинается со указанной
	static bool StartsWith(const std::string &str, const std::string &start);

	/// Проверяет, что строка заканчивается указанной
	static bool EndsWith(const std::string &str, const std::string &end);

	/// Добавляет перечисленные в строке группы ресурсов в коллекцию
	static void MergeGroups(const std::string& groups, std::set<std::string>& result);
};

} // namespace utils

#endif
