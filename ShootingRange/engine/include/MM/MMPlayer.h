#pragma once

#include "Render/Texture.h"

class TheoraVideoManager;
class TheoraAudioInterfaceFactory;
class TheoraVideoClip;

namespace MM {

/// Проигрыватель видео файлов
class MMPlayer
{
public:
	MMPlayer(const std::string& filename, bool loop = false, bool vsync = false, bool cacheToMemory = false);
	~MMPlayer();
	
	void FadeInTrack(float s);
	void FadeOutTrack(float s);
	
	void SetVolume(float volume);
	float GetVolume() const;
	
	void Play();
	bool IsPlaying() const;
	void Pause(bool pause);
	void Rewind();
	void Stop();
	
	float GetDuration() const;
	float GetTimePosition() const;
	void Seek(float time_position);
	
	bool Finished() const;

	void Update(float dt);
	void Draw(int x, int y, int width, int height);
	
	Render::Texture* GetVideoTexture() const { return _video.get(); }

private:
	std::string _filename;
	bool _loop;
	bool _vsync;

	static boost::shared_ptr<TheoraVideoManager> sharedMgr;
	boost::shared_ptr<TheoraVideoManager> _mgr;
	std::unique_ptr<Render::Texture> _video;
	TheoraVideoClip* _clip;

	bool _fadeIn;
	bool _fadeOut;
	float _fadeInTime;
	float _fadeOutTime;
	float _fadeLength;
	float _fadeVolume;
	
	bool _cacheToMemory;

private:
	// Запретить копирование и присваивание
	MMPlayer(const MMPlayer&) = delete;
	MMPlayer& operator=(const MMPlayer&) = delete;
	
	void Open();
	void Close();
};

} // namespace MM
