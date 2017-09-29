#ifndef __AUDIOSOURCE_H__
#define __AUDIOSOURCE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "RefCounter.h"
#include "IO/Stream.h"

namespace MM {

///
/// »сточник звуковых данных
///
/// ѕозвол€ет прочитать PCM данные из разнообразных входных форматов
///
class AudioSource : public IO::InputStream {
public:
	AudioSource() = default;
	
	/// ¬озвращает количество каналов (обычно 1 или 2)
	virtual size_t GetChannels() const = 0;
	
	/// ¬озвращает частоту дискретизации
	virtual size_t GetRate() const = 0;
	
	/// ¬озвращает количество бит на выборку (обычно 8, 16, 32)
	virtual size_t GetBits() const = 0;
	
	/// ¬озвращает размер данных в байтах
	virtual size_t GetLengthInBytes() const = 0;
	
	/// ѕровер€ет, достигнут ли конец данных
	/// (ѕри работе с неблокирующими источниками Read может возвращать 0 при недостигнутом конце)
	virtual bool IsEof() const = 0;
	
	/// ѕытаетс€ прочитать указанное количество байт из источника, пока не будет достигнут конец данных
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
