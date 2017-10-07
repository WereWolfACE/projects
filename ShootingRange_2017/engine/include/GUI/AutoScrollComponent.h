#pragma once

#include "GUI/ScrollableFrameModel.h"

namespace GUI {

//
// Компонент автоматической прокрутки фрейма
// (используется алгоритм, описанный в статье http://ariya.ofilabs.com/2011/10/flick-list-with-its-momentum-scrolling-and-deceleration.html)
//
class AutoScrollComponent {
public:
	struct TimeConstants {
		static const float normal;
		static const float fast;
	};

	AutoScrollComponent();

	void Init(ScrollableFrameModel* model);

	void Update(float dt);

	void SetTarget(const FPoint& target) { SetTarget(target, _timeConstant); }
	void SetTarget(const FPoint& target, float curTimeConstant);
	const FPoint& GetTarget() const { return _target; }

	void SetVelocity(const FPoint& velocity);

	void SetScaleFactor(float scaleFactor) { _scaleFactor = scaleFactor; }
	void SetTimeConstant(float timeConstant) { _timeConstant = timeConstant; }

	void Activate() { _active = true; }
	void Deactivate() { _active = false; }
	bool IsMoving() const;

private:
	ScrollableFrameModel* _model;

	FPoint _target;				//текущее целевое смещение прокрутки

	float _scaleFactor;			//характеризует расстояние, которое пройдёт прокрутка при известной начальной скорости (L = V0 * _scaleFactor)
	float _timeConstant;		//характеризует "динамичность" прокрутки (чем меньше _timeConstant, тем "динамичнее")
	float _curTimeConstant;		//текущее значение timeConstant

	FPoint _amplitude;			//расстояние, которое преодолеет прокрутка
	float _duration;			//длительность прокрутки
	float _minVelocity;			//минимальная начальная скорость, при которой требуется автопрокрутка

	bool _active;				//активна ли автопрокрутка
};

} // namespace GUI
