#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#ifdef _MSC_VER
#pragma once
#endif

#ifdef SetCurrentDirectory
#undef SetCurrentDirectory
#endif

#ifdef GetCurrentDirectory
#undef GetCurrentDirectory
#endif

#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

namespace IO {

enum class SearchMode {
	TopOnly,
	All
};

class Directory {
public:
	/// Проверяет существование директории
	static bool Exists(const std::string& dirname);
	
	/// Создаёт все поддиректории в пути, если какие-то из них не существуют
	static bool CreateDirectories(const std::string& dirname);
	
	/// Удаляет директорию и все вложенные файлы и поддиректории
	static bool RemoveDirectory(const std::string& dirname);

	/// Копирует директорию со всем содержимым
	static bool CopyDirectoryWithContents(const std::string& from, const std::string& to);

	/// Переносит директорию со всем содержимым
	static bool MoveDirectoryWithContents(const std::string& from, const std::string& to);

	/// Устанавливает текущую директорию процесса
	static bool SetCurrentDirectory(const std::string& dirname);

	/// Возвращает текущую директорию процесса
	static std::string GetCurrentDirectory();
	
	/// Перечисляет файлы в директории с использованием функции обратного вызова
	static void EnumerateFiles(const std::string& dirname, std::function<void(const std::string&)> callback, SearchMode mode = SearchMode::TopOnly);

	/// Перечисляет поддиректории в директории с использованием функции обратного вызова
	static void EnumerateDirectories(const std::string& dirname, std::function<void(const std::string&)> callback, SearchMode mode = SearchMode::TopOnly);
	
	/// Ищет все файлы в директории и добавляет их имена в коллекцию
	static void FindFiles(const std::string& dirname, std::vector<std::string>& files, SearchMode mode = SearchMode::TopOnly);
	
	/// Ищет файлы в директории по маске и добавляет их имена в коллекцию
	static void FindFiles(const std::string& dirname, const std::string& filemask, std::vector<std::string>& files, SearchMode mode = SearchMode::TopOnly);
};

} // namespace IO

#endif // __DIRECTORY_H__
