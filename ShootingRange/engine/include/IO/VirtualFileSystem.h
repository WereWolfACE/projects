#ifndef __VIRTUALFILESYSTEM_H__
#define __VIRTUALFILESYSTEM_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "IO/FileSystem.h"

namespace IO {

/// ����������� �������� �������.
/// ������������� ���������� ������ ������� � �������� ����������.
///
/// ��� ���� ����������� ������������ �������� ����� ������������ ������.
/// ����������� ������������t ���������� �����. ��� ������ ������������ ������ �� �����
/// ��������� /Users/xxxx/Library/... ������� ������������ fopen � ����������� �������.
class VirtualFileSystem : public FileSystem {
public:
	explicit VirtualFileSystem(const std::string& name);

	/// ������������� ���������� ��� ������, � ������� ����� �������� ������� OpenWrite/OpenUpdate
	void SetWriteDirectory(const std::string& dir);

	/// ���������� ���������� ��� ������
	const std::string& GetWriteDirectory() const;

	/// ��������� �������� �������.
	/// ����� ����� �������������� � ������� ������������ �������� ������.
	void Mount(FileSystem* fs);

	/// ��������� ���������� � ������� �� �����
	void MountDirectory(const std::string& path);

	/// ��������� zip �����
	void MountZip(const std::string& filename, const std::string& innerPath);

	/// ���������, �������������� �� �������� ������� � ��������� ������
	bool IsMounted(const std::string& name) const;

	/// ������������ �������� �������.
	void Unmount(const std::string& name);

	virtual const std::string& GetName() const override { return _name; }

	/// ��������� ������������� �����
	virtual bool FileExists(const std::string& filename) const override;

	/// ���� ����� �� �����
	virtual void FindFiles(const std::string& filespec, std::vector<std::string>& result) const override;

	/// ���������� ����� ��������� ������ �����
	virtual time_t GetLastWriteTime(const std::string& filename, boost::system::error_code& ec) const override;

	/// ��������� ���� ��� ������, ������� �������� ���������� ��� ������
	virtual InputStreamPtr OpenRead(const std::string& filename, boost::system::error_code& ec) override;

	/// ��������� ���� ��� ������ � ���������� ��� ������
	virtual OutputStreamPtr OpenWrite(const std::string& filename, boost::system::error_code& ec) override;

	/// ��������� ���� ��� ���������� (�� ������� ����������) � ���������� ��� ������
	virtual StreamPtr OpenUpdate(const std::string& filename, boost::system::error_code& ec) override;

	/// ������� ���� �� ���������� ��� ������
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
