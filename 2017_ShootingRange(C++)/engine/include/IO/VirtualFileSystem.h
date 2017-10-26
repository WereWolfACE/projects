#ifndef __VIRTUALFILESYSTEM_H__
#define __VIRTUALFILESYSTEM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "IO/FileSystem.h"

namespace IO {

/// Виртуальная файловая система.
/// Предоставляет однородный способ доступа к ресурсам приложения.
///
/// Все пути указываются относительно корневой точки монтирования систем.
/// Недопустимо использованиt абсолютных путей. Для чтения произвольных файлов на диске
/// наподобие /Users/xxxx/Library/... следует использовать fopen и аналогичные функции.
class VirtualFileSystem : public FileSystem {
public:
	explicit VirtualFileSystem(const std::string& name);

	/// Устанавливает директорию для записи, с которой будут работать функции OpenWrite/OpenUpdate
	void SetWriteDirectory(const std::string& dir);

	/// Возвращает директорию для записи
	const std::string& GetWriteDirectory() const;

	/// Монтирует файловую систему.
	/// Поиск файла осуществляется в порядке монтирования файловых систем.
	void Mount(FileSystem* fs);

	/// Монтирует директорию с файлами на диске
	void MountDirectory(const std::string& path);

	/// Монтирует zip архив
	void MountZip(const std::string& filename, const std::string& innerPath);

	/// Проверяет, примонтирована ли файловая система с указанным именем
	bool IsMounted(const std::string& name) const;

	/// Размонтирует файловую систему.
	void Unmount(const std::string& name);

	virtual const std::string& GetName() const override { return _name; }

	/// Проверяет существование файла
	virtual bool FileExists(const std::string& filename) const override;

	/// Ищет файлы по маске
	virtual void FindFiles(const std::string& filespec, std::vector<std::string>& result) const override;

	/// Возвращает время последней записи файла
	virtual time_t GetLastWriteTime(const std::string& filename, boost::system::error_code& ec) const override;

	/// Открывает файл для чтения, сначала проверяя директорию для записи
	virtual InputStreamPtr OpenRead(const std::string& filename, boost::system::error_code& ec) override;

	/// Открывает файл для записи в директории для записи
	virtual OutputStreamPtr OpenWrite(const std::string& filename, boost::system::error_code& ec) override;

	/// Открывает файл для обновления (не удаляет содержимое) в директории для записи
	virtual StreamPtr OpenUpdate(const std::string& filename, boost::system::error_code& ec) override;

	/// Удаляет файл из директории для записи
	virtual void RemoveFile(const std::string& filename, boost::system::error_code& ec) override;

	using FileSystem::GetLastWriteTime;
	using FileSystem::OpenRead;
	using FileSystem::OpenWrite;
	using FileSystem::OpenUpdate;
	using FileSystem::RemoveFile;

private:
	std::string _name;

	typedef std::list<FileSystemPtr> FileSystems;
	FileSystems _systems;

private:
	DISALLOW_COPY_AND_ASSIGN(VirtualFileSystem);
};

} // namespace IO


namespace Core {
	extern IO::VirtualFileSystem fileSystem;
}

#endif // __VIRTUALFILESYSTEM_H__
