#ifndef __ANIMATIONCHANNEL_H__
#define __ANIMATIONCHANNEL_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "SceneGraph/SceneNode.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Канал анимации. Описывает анимацию одного узла сцены.
///
class AnimationChannel : public RefCounter {
public:
	typedef boost::intrusive_ptr<AnimationChannel> HardPtr;

	explicit AnimationChannel(const std::string& name);

	// AnimationChannel(const AnimationChannel& rhs);
	// AnimationChannel& operator = (const AnimationChannel&);
		//
		// Конструктор копий и оператор присваивания подходят те,
		// что автоматически создаст компилятор.

	virtual ~AnimationChannel();

	virtual AnimationChannel::HardPtr Clone() = 0;

	size_t GetMemoryInUse() const;

	const std::string& GetName() const;
		/// Возвращает имя канала

	void SetName(const std::string& name);
		/// Устанавливает имя канала

	virtual void OnAnimate(SceneNode* node, float dt) = 0;
		/// Вызывается для анимирования узла

protected:
	std::string _name;
		/// Имя канала анимации, уникальное в пределах одной анимации
};

} // namespace SceneGraph

#endif // __ANIMATIONCHANNEL_H__
