#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "SceneGraph/AnimationChannel.h"
#include "SceneGraph/Delegate.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Состояние анимации.
///
enum class AnimationState {
	Stopped,
		/// Остановлена
	Playing,
		/// Проигрывается
	Paused
		/// Приостановлена
};

///
/// Подсказка завершения анимации.
///
enum class AnimationFinishHint {
	Begin,
		/// Завершилась в начале
	End
		/// Завершилась в конце
};

///
/// Поведение анимации после достижения конца.
///
enum class AnimationFinishBehaviour {
	Stop,
		/// Остановиться
	Pause,
		/// Встать на паузу
	Continue,
		/// Продолжить без изменения
	PingPong
		/// Продолжить в обратном направлении
};

///
/// Аргументы события OnStarted
///
class AnimationStartedEventArgs : public EventArgs {
public:
};

///
/// Аргументы события OnFinished
///
class AnimationFinishedEventArgs : public EventArgs {
public:
	AnimationFinishedEventArgs(AnimationFinishHint hint, AnimationFinishBehaviour behaviour);

	AnimationFinishHint FinishedAt() const;
		/// Возвращает место остановки

	AnimationFinishBehaviour GetBehaviour() const;
		/// Возвращает поведение после остановки

	void SetBehaviour(AnimationFinishBehaviour behaviour);
		/// Устанавливает поведение после остановки

private:
	AnimationFinishHint _hint;
		/// Подсказка завершения

	AnimationFinishBehaviour _behaviour;
		/// Поведение после завершения
};

///
/// Аргументы события OnStateChanged
///
class AnimationStateChangedEventArgs : public EventArgs {
public:
	AnimationStateChangedEventArgs(AnimationState oldState, AnimationState newState);

	AnimationState OldState() const;
		/// Возвращает прежнее состояние анимации

	AnimationState NewState() const;
		/// Возвращает новое состояние анимации

private:
	AnimationState _oldState;
		/// Прежнее состояние анимации

	AnimationState _newState;
		/// Новое состояние анимации
};

///
/// Аргументы события OnAdvanceTime
///
class AnimationAdvanceTimeEventArgs : public EventArgs {
public:
	explicit AnimationAdvanceTimeEventArgs(float deltaTime);

	float DeltaTime() const;
		/// Возвращает время, на сколько продвинулась анимация

private:
	float _deltaTime;
		/// Время продвижения анимации
};

///
/// Анимация.
///
class Animation : public RefCounter {
public:
	typedef boost::intrusive_ptr<Animation> HardPtr;

	typedef std::multimap<SceneNode*, AnimationChannel::HardPtr> ChannelsMap;

	explicit Animation(const std::string& name);

	virtual ~Animation();

	virtual Animation::HardPtr Clone();
		/// Клонирует экземпляр объекта

	virtual size_t GetMemoryInUse() const;

	const std::string& GetName() const;
		/// Возвращает имя анимации

	AnimationState GetState() const;
		/// Возвращает состояние анимации

	AnimationFinishBehaviour GetBehaviour() const;
		/// Возвращает поведение анимации после достижения конца

	void SetBehaviour(AnimationFinishBehaviour behaviour);
		/// Устанавливает поведение анимации после достижения конца

	float GetSpeed() const;
		/// Возвращает скорость проигрывания анимации

	void SetSpeed(float speed);
		/// Устанавливает скорость проигрывания анимации

	float GetBlend() const;
		/// Возвращает коэффициент смешивания анимации

	void SetBlend(float blend);
		/// Устанавливает коэффициент смешивания анимации

	bool IsInversed() const;
		/// Позволяет узнать, инвертирована ли анимация

	virtual void SetInversed(bool value);
		/// Устанавливает инвертирование анимации

	ChannelsMap& GetChannels();
		/// Возвращает коллекцию каналов анимации

	void RemoveAllSubscribers();
		// Удалить все делегаты.

	void FindChannels(SceneNode* node, std::vector<AnimationChannel::HardPtr>& channels) const;
		/// Ищет каналы анимации, ассоциированные с указанным узлом сцены

	AnimationChannel::HardPtr FindChannel(SceneNode* node, const std::string& name) const;
		/// Ищет канал анимации с указанным именем, ассоциированный с указанным узлом сцены

	void AddChannel(SceneNode* node, AnimationChannel::HardPtr channel);
		/// Добавляет канал анимации

	bool RemoveChannel(SceneNode* node, AnimationChannel::HardPtr channel);
		/// Удаляет канал анимации

	bool RemoveChannel(SceneNode* node, const std::string& name);
		/// Удаляет канал анимации с указанным именем

	bool RemoveChannels(SceneNode* node);
		/// Удаляет все каналы анимации для указанного узла сцены

	virtual bool CopyChannel(Animation::HardPtr src, SceneNode* node);
		/// Копируем канал из другой анимации

	virtual void Play();
		/// Начинает проигрывание анимации. Продолжает, если была на паузе

	virtual void Pause();
		/// Ставит проигрывание анимации на паузу

	virtual void Stop();
		/// Останавливает проигрывание анимации

	virtual void AdvanceTime(float dt);
		/// Продвигает проигрывание анимации на указанное время

	virtual void SetFrame(int /*frame*/) {};
		/// устанавливаем номер фрейма

	virtual float GetNormalizedPosition() const { return 0; };
		/// возвращает продолжительность анимации от 0 до 1

	Event<AnimationStartedEventArgs> OnStarted;
		/// Вызывается при старте анимации

	Event<AnimationFinishedEventArgs> OnFinished;
		/// Вызывается при завершении анимации

	Event<AnimationStateChangedEventArgs> OnStateChanged;
		/// Вызывается при изменении состояния анимации

	Event<AnimationAdvanceTimeEventArgs> OnAdvanceTime;
		/// Вызывается при продвижении анимации

	virtual void SetModelNode(ModelNode* modelNode);
	ModelNode* GetModelNode() { return _modelNode; }

protected:
	Animation(const Animation& rhs);

	Animation& operator = (const Animation& rhs);

protected:
	ModelNode* _modelNode;

	std::string _name;
		/// Имя анимации.
		/// Должно быть уникальным в пределах одной модели.

	ChannelsMap _channels;
		/// Каналы анимации

	AnimationState _state;
		/// Состояние анимации

	AnimationFinishBehaviour _behaviour;
		/// Поведение анимации после достижения конца

	float _speed;
		/// Скорость проигрывания анимации

	float _blend;
		/// Коэффициент смешивания анимации

	bool _inversed;
		/// Инвертирована ли анимация
};

} // namespace SceneGraph

#endif // __ANIMATION_H__
