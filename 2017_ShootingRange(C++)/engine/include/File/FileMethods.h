#ifndef __FILE_FILEMETHODS_H__
#define __FILE_FILEMETHODS_H__

#pragma once

#include "IO/Stream.h"

#include <string>

#include <boost/system/error_code.hpp>

#ifdef CopyFile
#undef CopyFile
#endif

#ifdef MoveFile
#undef MoveFile
#endif

enum class FileNameOp {
	Resolve,
	DontResolve
};

enum class ErrorMode {
	ReturnCode,
	ThrowException
};

namespace File {

/// Проверяет существование файла в файловой системе
bool ExistsInFs(const std::string& filename);

/// Копирует файл в файловой системе.
/// Если файл существует, то он не перезаписывается и возвращается false.
bool CopyFile(const std::string& from, const std::string& to);

/// Перемещает файл в файловой системе.
/// Если файл существует, то он не перезаписывается и возвращается false.
bool MoveFile(const std::string& from, const std::string& to);

/// Удаляет файл в файловой системе
bool RemoveFile(const std::string& file);

/// Удаляет файлы в файловой системе по указанному пути
bool RemoveAll(const std::string& path);

/// Преобразует имя файла в ВИРТУАЛЬНОЙ файловой системе с учётом текущего выбранного языка
std::string ResolveFileName(const std::string& filename);

/// Открывает файл для чтения в ВИРТУАЛЬНОЙ файловой системе
IO::InputStreamPtr OpenRead(const std::string& filename, FileNameOp op = FileNameOp::DontResolve, ErrorMode err = ErrorMode::ThrowException);

/// Загружает содержимое файла из ВИРТУАЛЬНОЙ файловой системы
bool LoadFile(const std::string& filename, std::vector<uint8_t>& contents, FileNameOp op = FileNameOp::DontResolve);

} // namespace File

#endif // __FILE_FILEMETHODS_H__
