#ifndef _RENDER_STREAMINGANIMATION_H_
#define _RENDER_STREAMINGANIMATION_H_

#include "Render/Texture.h"
#include "Core/Resource.h"
#include "Utils/utils.hpp"

#include <boost/smart_ptr.hpp>

namespace Render {

namespace Detail {

///
/// Провайдер данных анимации
///
class AnimationDataProvider {
public:
	typedef boost::shared_ptr<AnimationDataProvider> HardPtr;
	typedef boost::weak_ptr<AnimationDataProvider> WeakPtr;

	explicit AnimationDataProvider(const std::string& path)
		: _path(path)
		, _fpsFactor(1.0f)
		, _origWidth(0)
		, _origHeight(0)
		, _frameWidth(0)
		, _frameHeight(0)
		, _frameCount(0)
	{
	}

	virtual ~AnimationDataProvider() {}

	float GetFpsFactor() const { return _fpsFactor; }
	int GetOrigWidth() const { return _origWidth; }
	int GetOrigHeight() const { return _origHeight; }
	int GetFrameWidth() const { return _frameWidth; }
	int GetFrameHeight() const { return _frameHeight; }
	int GetFrameCount() const { return _frameCount; }
	const IPoint& GetOffset() const { return _offset; }

	virtual void Upload() = 0;
	virtual void Release() = 0;

	virtual void UpdateColorBuffer(int frame) = 0;
		/// Вызывается при необходимости обновить буфер кадра анимации
	virtual void RenderFrameTo(int frame, uint8_t* data, int pitch) = 0;
		/// Вызывается при необходимости отрисовать кадр анимации
	virtual bool IsPixelOpaque(int frame, int x, int y) = 0;
		/// Проверяет пиксель текущего кадра на прозрачность
	virtual size_t GetMemoryInUse() const = 0;
		/// Возвращает размер используемой памяти

protected:
	std::string _path;
	float _fpsFactor;
	int _origWidth;
	int _origHeight;
	int _frameWidth;
	int _frameHeight;
	int _frameCount;
	IPoint _offset;
};

} // namespace Detail

typedef boost::intrusive_ptr<class StreamingAnimation> StreamingAnimationPtr;

///
/// Класс анимации, работающий по принципу потокового воспроизведения,
/// т.е. в видеопамять записывается только текущий кадр анимации.
///
/// Призван экономить видеопамять в обмен на FPS.
///
class StreamingAnimation : public Resource {
public:
	enum class AnimationMode {
		Forward = 0,
		Backward = 1,
		PingPong = 2,
		Loop = 4
	};

	StreamingAnimation();
	explicit StreamingAnimation(rapidxml::xml_node<>* elem);
	StreamingAnimation(const StreamingAnimation&);

	StreamingAnimation& operator=(const StreamingAnimation&);

	virtual ~StreamingAnimation();

	virtual StreamingAnimation* Clone() const;

	virtual void Upload();
	virtual void Release();

	const std::string& GetName() const { return _id; }

	size_t GetMemoryInUse() const;

	const std::string& GetId() const;
	const std::string& GetDir() const;
	void SetFilterringType(FilteringType filter);
	void Bind(int channel = 0);
	void Update(float dt);
	void Draw(SpriteBatch* batch = nullptr);
	void Draw(const IPoint& position, SpriteBatch* batch = nullptr);
	void MoveTo(const IPoint& position);
	bool IsMirrored() const;
	void SetMirrored(bool mirrored);
	int GetFrameWidth() const;
	int GetFrameHeight() const;
	int GetOrigWidth() const;
	int GetOrigHeight() const;
	int GetFrameCount() const;
	int GetCurrentFrame() const;
	void SetCurrentFrame(int frame);
	void SetFirstPlayedFrame(int frame);
	void SetLastPlayedFrame(int frame);
	int GetFirstPlayedFrame() const;
	int GetLastPlayedFrame() const;
	float GetSpeed() const;
	void SetSpeed(float fps);
	AnimationMode GetMode() const;
	void SetMode(AnimationMode mode);
	void SetPlayback(bool playback);
	bool IsPlaying() const;
	bool IsFinished() const;
	void SetLoop(bool loop);
	bool isPixelOpaque(int x, int y);
	bool isPixelOpaque(const IPoint& p) { return isPixelOpaque(p.x, p.y); }

	static StreamingAnimationPtr Spawn(const std::string& id);

protected:
	void InitFrameBuffer();

protected:
	std::string _id;
	std::string _dir;

	int _origWidth;
	int _origHeight;
	int _frameWidth;
	int _frameHeight;
	int _frameCount;
	int _currentFrame;
	int _firstPlayedFrame;
	int _lastPlayedFrame;
	bool _isPlaying;
	bool _isFinished;
	float _speed;
	float _fpsFactor;
	float _sinceChangeTime;
	int _delta;
	AnimationMode _mode;
	bool _needReload;
	IPoint _position;
	bool _mirrored;
	IPoint _offset;

	boost::intrusive_ptr<class Framebuffer> _frameBuffer;

	Detail::AnimationDataProvider::HardPtr _provider;
};

DECLARE_ENUM_CLASS_FLAGS(StreamingAnimation::AnimationMode)

//
///////////////////////////////////////////////////////////////////////////

inline const std::string& StreamingAnimation::GetId() const {
	return _id;
}

inline const std::string& StreamingAnimation::GetDir() const {
	return _dir;
}

inline void StreamingAnimation::SetFirstPlayedFrame(int frame) {
	_firstPlayedFrame = (frame >= 0 && frame <= _frameCount - 1) ? frame : 0;
}

inline void StreamingAnimation::SetLastPlayedFrame(int frame) {
	_lastPlayedFrame = (frame >= 0 && frame <= _frameCount - 1) ? frame : _frameCount - 1;
}

inline int StreamingAnimation::GetFirstPlayedFrame() const {
	return _firstPlayedFrame;
}

inline int StreamingAnimation::GetLastPlayedFrame() const {
	return _lastPlayedFrame;
}

inline void StreamingAnimation::Draw(const IPoint& position, SpriteBatch* batch) {
	MoveTo(position);
	Draw(batch);
}

inline int StreamingAnimation::GetFrameWidth() const {
	return _frameWidth;
}

inline int StreamingAnimation::GetFrameHeight() const {
	return _frameHeight;
}

inline int StreamingAnimation::GetOrigWidth() const {
	return _origWidth;
}

inline int StreamingAnimation::GetOrigHeight() const {
	return _origHeight;
}

inline void StreamingAnimation::MoveTo(const IPoint& position) {
	_position = position;
}

inline bool StreamingAnimation::IsMirrored() const {
	return _mirrored;
}

inline void StreamingAnimation::SetMirrored(bool mirrored) {
	if (_mirrored != mirrored) {
		_mirrored = mirrored;
		_needReload = true;
	}
}

inline void StreamingAnimation::SetLoop(bool loop)
{
	if (loop) {
		_mode = _mode | AnimationMode::Loop;
	} else {
		_mode = _mode & (~AnimationMode::Loop);
	}
}

inline int StreamingAnimation::GetFrameCount() const {
	return _frameCount;
}

inline int StreamingAnimation::GetCurrentFrame() const {
	return _currentFrame;
}

inline float StreamingAnimation::GetSpeed() const {
	return _speed;
}

inline void StreamingAnimation::SetSpeed(float fps) {
	_speed = 1.0f / fps;
}

inline StreamingAnimation::AnimationMode StreamingAnimation::GetMode() const {
	return _mode;
}

inline bool StreamingAnimation::IsPlaying() const {
	return _isPlaying;
}

inline bool StreamingAnimation::IsFinished() const {
	return !_isPlaying || _isFinished;
}

} // namespace Render

#endif // _RENDER_STREAMINGANIMATION_H_
