#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "IO/Stream.h"

namespace IO {

class FileSystem : public RefCounter {
public:
	explicit FileSystem(const std::string& root);

	const std::string& GetRoot() const { return _root; }

	virtual const std::string& GetName() const = 0;

	virtual bool FileExists(const std::string& filename) const = 0;

	virtual void FindFiles(const std::string& filespec, std::vector<std::string>& result) const = 0;

	virtual time_t GetLastWriteTime(const std::string& filename, boost::system::error_code& ec) const = 0;

	virtual InputStreamPtr OpenRead(const std::string& filename, boost::system::error_code& ec) = 0;

	virtual OutputStreamPtr OpenWrite(const std::string& filename, boost::system::error_code& ec) = 0;

	virtual StreamPtr OpenUpdate(const std::string& filename, boost::system::error_code& ec) = 0;

	virtual void RemoveFile(const std::string& filename, boost::system::error_code& ec) = 0;

	time_t GetLastWriteTime(const std::string& filename) const {
		boost::system::error_code ec;
		auto ret = GetLastWriteTime(filename, ec);
		return !ec ? ret : -1;
	}

	InputStreamPtr OpenRead(const std::string& filename) {
		boost::system::error_code ec;
		auto ret = OpenRead(filename, ec);
		return !ec ? ret : nullptr;
	}

	OutputStreamPtr OpenWrite(const std::string& filename) {
		boost::system::error_code ec;
		auto ret = OpenWrite(filename, ec);
		return !ec ? ret : nullptr;
	}

	StreamPtr OpenUpdate(const std::string& filename) {
		boost::system::error_code ec;
		auto ret = OpenUpdate(filename, ec);
		return !ec ? ret : nullptr;
	}

	bool RemoveFile(const std::string& filename) {
		boost::system::error_code ec;
		RemoveFile(filename, ec);
		return !ec;
	}

private:
	std::string _root;

private:
	DISALLOW_COPY_AND_ASSIGN(FileSystem)
};

typedef boost::intrusive_ptr<FileSystem> FileSystemPtr;


} // namespace IO

#endif // __FILESYSTEM_H__
