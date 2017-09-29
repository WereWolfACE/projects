#ifndef __PATH_H__
#define __PATH_H__

#ifdef _MSC_VER
#pragma once
#endif

enum class SpecialFolder {
	LocalDocuments, ///< Директория для записи данных, создаваемых пользователем
	LocalData,      ///< Директория для записи невозобновляемых данных приложения (базовая для Updates, Caches и др.)
	LocalUpdates,   ///< Директория для записи обновлений приложения
	LocalCaches,    ///< Директория для записи возобновляемых данных приложения (данные могут быть удалены, не копируются в iTunes)
};

namespace IO {

class Path {
public:
	static const std::string DirectorySeparator;
	static const std::string AltDirectorySeparator;
	static const std::string FileExtensionSeparator;

	/// Объединяет две части пути
	static std::string Combine(const std::string& part1, const std::string& part2);
	static std::string Combine(std::initializer_list<const char*> parts);
	static std::string Combine(std::initializer_list<std::string> parts);
	/// Возвращает имя файла без расширения
	static std::string RemoveExtension(const std::string& path);
	/// Изменяет расширение файла
	static std::string ChangeExtension(const std::string& path, const std::string& extension);
	/// Возвращает часть пути без файла
	static std::string GetDirectoryName(const std::string& path);
	/// Возвращает имя файла без пути
	static std::string GetFileName(const std::string& path);
	/// Разбивает путь на составляющие части
	static void Split(const std::string& path, std::vector<std::string>& parts);
	/// Разрешает все "..", перенося излишек в начало пути, и удаляет все "."
	static std::string Canonicalize(const std::string& path);
	/// Меняет все разделители пути на прямые
	static void MakeSlashesForward(std::string& path);
	static std::string MakeSlashesForwardCopy(const std::string& path);
	/// Меняет все разделители пути на обратные
	static void MakeSlashesBackward(std::string& path);
	static std::string MakeSlashesBackwardCopy(const std::string& path);
	/// Проверяет имя пути на соответствие маске, содержащей подстановочные знаки '.', '?' и '*'
	static bool MatchesWild(const std::string& path, const std::string& wild);
	/// Возвращает путь к особой директории
	static std::string GetSpecialFolderPath(SpecialFolder folder);
	/// Проверяет, переносим ли путь на другие платформы (если нет, но сработает Assert)
	static void CheckPathPortability(const std::string& path, const std::string& sourceFilePath = "");
};

} // namespace IO

#endif // __PATH_H__
