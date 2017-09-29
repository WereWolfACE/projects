#pragma once

#include "GUI/ScrollableFrameModel.h"

namespace GUI {

//
// ��������� �������������� ��������� ������
// (������������ ��������, ��������� � ������ http://ariya.ofilabs.com/2011/10/flick-list-with-its-momentum-scrolling-and-deceleration.html)
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

	FPoint _target;				//������� ������� �������� ���������

	float _scaleFactor;			//������������� ����������, ������� ������ ��������� ��� ��������� ��������� �������� (L = V0 * _scaleFactor)
	float _timeConstant;		//������������� "������������" ��������� (��� ������ _timeConstant, ��� "����������")
	float _curTimeConstant;		//������� �������� timeConstant

	FPoint _amplitude;			//����������, ������� ���������� ���������
	float _duration;			//������������ ���������
	float _minVelocity;			//����������� ��������� ��������, ��� ������� ��������� �������������

	bool _active;				//������� �� �������������
};

} // namespace GUI
