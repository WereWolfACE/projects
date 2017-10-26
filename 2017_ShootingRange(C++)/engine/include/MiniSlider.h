#ifndef __MINISLIDER_H__
#define __MINISLIDER_H__

#pragma once

#include "Render.h"

//
//  ласс минислайдера - слайдер, который отрисовываетс€ без текстур.
//
class MiniSlider
{
public:
	///  онструктор
	MiniSlider(IPoint pos, int size, bool isVertical, int begunWidth, int begunHeight);

	/// ¬ернуть относительное положение бегунка (от 0 до 1)
	float GetFactor() const;

	/// ”становить относительное положение бегунка (от 0 до 1)
	void SetFactor(float factor);

	/// ќтрисовать слайдер
	void Draw();

	/// ќбработать нажатие мыши
	bool MouseDown(const IPoint &mousePos);

	/// ќбработать движение мыши
	void MouseMove(const IPoint &mousePos);

	/// ќбработать отжатие мыши
	void MouseUp(const IPoint &mousePos);

	/// ”становить длину слайдера
	void SetSize(int size);

	/// ”становить позицию слайдера
	void SetPosition(IPoint pos);

private:

	/// вертикальный/горизонтальный
	bool _isVertical;

	/// координаты левого нижнего кра€ линии, вдоль которой перемещаетс€ бегунок
	IPoint _pos;

	/// длина линии
	int _size;

	/// высота бегунка
	int _begunHeight;

	/// ширина бегунка
	int _begunWidth;

	/// текуща€ позици€ бегунка
	int _begunPos;

	/// нажата ли кнопка
	bool _isMouseDown;

};

#endif // __MINISLIDER_H__
