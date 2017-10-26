#pragma once

#include <RefCounter.h>
#include <Render/RenderDevice.h>

#include <boost/intrusive_ptr.hpp>

#include "Flash/core/FlashUpdateListener.hpp"

struct IFlashDisplayObject;
struct IFlashMovieClip;

/**
* Класс для работы с Flash-анимацией в обход виджетов.
* Для использования совместно с fQuery не подходит, рекомендуется только если вы
* используете flash объекты как анимацию и не планируете делать их интерактивными.
*/
class FlashAnimation : public RefCounter {
public:
	FlashAnimation(const std::string &libId, const std::string &animationId);
	explicit FlashAnimation(IFlashMovieClip* _movieclip);
	virtual ~FlashAnimation();

	///Устанавливает центр объекта (точку, относительно которой будет рисоваться объект)
	void SetHotSpot(float x, float y);
	IPoint GetHotSpot() const { return IPoint((int)_xSpot, (int)_ySpot); }

	void Draw(float x, float y, bool mirror = false);
	void Update(float dt);

	bool IsLastFrame() const;
	void Reset();

	/**Устанавливает воспроизведение в обратном порядке.
	* Данная функция не рекомендуется к использованию. Если вам действительно нужно
	* воспроизводить анимации наоборот, обратитесь ко мне и я добавлю эту фичу более
	* хорошим способом. @Денис Степанов
	*/
	void SetReversed(bool rev);
	bool IsReversed() const { return _reversed; }

	int GetCurrentFrame() const;
	int GetLastFrame() const;

	inline IFlashMovieClip* getFlashMovieClip() const { return _movieClip; }

	//deprecated
	IFlashDisplayObject* getDisplayObject() const;

private:
	float _xSpot;
	float _ySpot;
	IFlashMovieClip* _movieClip;
	bool _reversed;
	FlashUpdateListener _updateListener;
};

typedef boost::intrusive_ptr<FlashAnimation> FlashAnimationPtr;
