#ifndef __ZIPARCHIVE_H__
#define __ZIPARCHIVE_H__

#pragma once

///
/// Класс для чтения Zip архивов.
///
/// В случае ошибок бросает исключение std::runtime_error.
///
class ZipArchive {
public:
	ZipArchive();
	explicit ZipArchive(const std::string& filename);
	explicit ZipArchive(const IO::InputStreamPtr& stream);

	virtual ~ZipArchive();

	void Open(const std::string& filename);
	void Open(const IO::InputStreamPtr& stream);
		/// Открывает архив. Открытый ранее архив автоматически закрывается

	void Close();
		/// Закрывает архив. Так же вызывается в деструкторе
	bool IsOpen() const;
		/// Проверяет, открыт ли архив

	void ListFiles(std::vector<std::string>& vec);
		/// Составляет список всех файлов, содержащихся в архиве
	bool FileExists(const std::string& filename);
		/// Проверяет, содержится ли файл в архиве
	void ReadFile(const std::string& filename, std::vector<uint8_t>& data);
		/// Читает содержимое файла в буфер. Добавляет к данным указанное количество нулей

private:
	ZipArchive(const ZipArchive&);
	ZipArchive& operator=(const ZipArchive&);
		// Запрещает копирование и присваивание экземпляров класса

private:
	struct Impl;
	std::unique_ptr<Impl> _pimpl;
		// Внутренняя реализация
};

#endif // __ZIPARCHIVE_H__
