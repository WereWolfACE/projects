#ifndef __KEYFRAMEDANIMATION_H__
#define __KEYFRAMEDANIMATION_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "SceneGraph/Animation.h"
#include "SceneGraph/KeyFramedAnimationChannel.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Анимация, основанная на ключевых кадрах.
///
class KeyFramedAnimation : public Animation {
public:
	typedef boost::intrusive_ptr<KeyFramedAnimation> HardPtr;
	
	KeyFramedAnimation(const std::string& name, float duration, float ticksPerSec);

	virtual Animation::HardPtr Clone() override;
	
	bool CopyChannel(Animation::HardPtr src, SceneNode* node) override;
	
	float GetDuration() const;
		/// Возвращает продолжительность анимации в тиках
	
	float GetTicksPerSec() const;
		/// Возвращает количество тиков в секунду
	
	float GetAdvanceTime() const;
		/// Возвращает время продвижения анимации в секундах
	
	void AdvanceTime(float dt) override;
		/// Продвигает проигрывание анимации на указанное время
	
	void SetInversed(bool inversed) override;
		// запускает анимацию в обратном порядке	

	virtual void Stop() override;
		/// Останавливает проигрывание анимации

	void SetFrame(int frame) override;

	float GetNormalizedPosition() const override;


protected:
	KeyFramedAnimation(const KeyFramedAnimation& rhs);
	
	KeyFramedAnimation& operator = (const KeyFramedAnimation& rhs);
	
protected:
	float _duration;
		/// Продолжительность анимации в тиках
	
	float _ticksPerSec;
		/// Количество тиков в секунду
	
	float _advanceTime;
		/// Время продвижения анимации в секундах
};


} // namespace SceneGraph

#endif // __KEYFRAMEDANIMATION_H__
