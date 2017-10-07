#pragma once

#include "GUI/ScrollableFrameModel.h"

namespace GUI {

//
// Компонент прокрутки фрейма пользователем
//
class ManualScrollComponent {
public:
	typedef std::function<void(const FPoint & lastVelocity)> OnEndHandler;

	ManualScrollComponent();

	void Init(ScrollableFrameModel* model);

	void Update(float dt);

	bool MouseDown(const IPoint& mouse_pos);
	void MouseUp(const IPoint& mouse_pos);
	void MouseMove(const IPoint& mouse_pos);

	void AddOnEndHandler(OnEndHandler handler) { _onEnd.push_back(handler); }

	bool InScroll() const { return _scrollBegin; }
	bool Pressed() const { return _pressed; }

	FPoint GetContentShiftPressed() const { return _contentShiftPressed; }

private:
	void RunOnEnd(const FPoint& lastVelocity);

private:
	ScrollableFrameModel* _model;

	bool _pressed;					// Было ли нажатие в окне прокрутки
	bool _scrollBegin;				// Начата ли прокрутка

	IPoint _posPressed;				// Координаты нажатия
	FPoint _contentShiftPressed;	// Смещение содержимого в момент нажатия

	float _minScrollLength;			// Минимальное смещение, необходимое чтобы прокрутка началась

	IPoint _lastMousePos;			// Последние координаты мыши при прокрутке
	IPoint _prevMousePos;			// Координаты мыши при прокрутке при предыдущем измерении скорости

	FPoint _velocity;				// Скорость прокрутки

	std::vector<OnEndHandler> _onEnd;	// Функции, вызываемые при завершении ручной пракрутки
};

} // namespace GUI
