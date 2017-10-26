#ifndef __AUDIOSOURCE_H__
#define __AUDIOSOURCE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "RefCounter.h"
#include "IO/Stream.h"

namespace MM {

///
/// �������� �������� ������
///
/// ��������� ��������� PCM ������ �� ������������� ������� ��������
///
class AudioSource : public IO::InputStream {
public:
	AudioSource() = default;
	
	/// ���������� ���������� ������� (������ 1 ��� 2)
	virtual size_t GetChannels() const = 0;
	
	/// ���������� ������� �������������
	virtual size_t GetRate() const = 0;
	
	/// ���������� ���������� ��� �� ������� (������ 8, 16, 32)
	virtual size_t GetBits() const = 0;
	
	/// ���������� ������ ������ � ������
	virtual size_t GetLengthInBytes() const = 0;
	
	/// ���������, ��������� �� ����� ������
	/// (��� ������ � �������������� ����������� Read ����� ���������� 0 ��� ������������� �����)
	virtual bool IsEof() const = 0;
	
	/// �������� ��������� ��������� ���������� ���� �� ���������, ���� �� ����� ��������� ����� ������
	void ReadSome(std::vector<uint8_t>& buffer, size_t size);

    static AudioSource* FromStream(IO::InputStream* stream);
	
	static AudioSource* FromBuffer(const boost::shared_ptr<std::vector<uint8_t>>& buffer, int channels, int rate, int bits);
	
	static AudioSource* FromFileOgg(const std::string& filename);
	
#if defined(ENGINE_TARGET_IPHONE) || defined(ENGINE_TARGET_MACOS)
	static AudioSource* FromFileAiff(const std::string& filename);
#endif
	
	static AudioSource* FromFile(const std::string& filename);

private:
	AudioSource(const AudioSource&) = delete;
	AudioSource& operator=(const AudioSource&) = delete;
};

typedef boost::intrusive_ptr<AudioSource> AudioSourcePtr;

} // namespace MM

#endif // __AUDIOSOURCE_H__
