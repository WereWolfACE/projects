#pragma once

#include "GUI/ScrollableFrameModel.h"

namespace GUI {

//
// ��������� ��������� ������ �������������
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

	bool _pressed;					// ���� �� ������� � ���� ���������
	bool _scrollBegin;				// ������ �� ���������

	IPoint _posPressed;				// ���������� �������
	FPoint _contentShiftPressed;	// �������� ����������� � ������ �������

	float _minScrollLength;			// ����������� ��������, ����������� ����� ��������� ��������

	IPoint _lastMousePos;			// ��������� ���������� ���� ��� ���������
	IPoint _prevMousePos;			// ���������� ���� ��� ��������� ��� ���������� ��������� ��������

	FPoint _velocity;				// �������� ���������

	std::vector<OnEndHandler> _onEnd;	// �������, ���������� ��� ���������� ������ ���������
};

} // namespace GUI
