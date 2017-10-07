#pragma once

#include "Flash/events/FlashEvent.h"

///������� ������ �������
extern const std::string& TouchBegin;
///������� ��������� �������
extern const std::string& TouchEnd;
///������� ����������� �������
extern const std::string& TouchMove;
///�������, ����������, ��� ����� ������� �������� ������� �������
extern const std::string& TouchLeave;
///�������, ����������, ��� ����� ������� ����� � ������� �������
extern const std::string& TouchEnter;
///������� ����������� ������� ���� ��� ������� ������
extern const std::string& TouchPassiveMove;
///�������, ����������, ��� ������� ���� ��������
extern const std::string& TouchCancel;

///�������, ����������, ��� ������� �������� ������� ����� (���� ����������)
extern const std::string& TouchLeaveStage;
///�������, ����������, ��� ������� ��������� � ������� �����
extern const std::string& TouchEnterStage;

///�������, ����������, ��� ����� ������� ����� � ������� �������.
///�������� � TouchEnter � ���, ��� TouchRollOver �� ����� ���� ������� � ����� � ���������.
///��������� ����������� � ������� MouseEvent.MOUSE_OVER / MouseEvent.ROLL_OVER � flash player.
///������ ����� ��� ����� ������ TouchRollOver.
extern const std::string& TouchRollOver;
///�������, ����������, ��� ����� ������� �������� ������� �������.
extern const std::string& TouchRollOut;

///������� ��������� �����
extern const std::string& GesturePinch;

class TouchEvent: public FlashEvent{
public:
	TouchEvent(const std::string& type, int touchIndex, float globalX, float globalY);

	/**
	* ID ������� - ��� ��������� multitouch �����������
	*/
	int getTouchIndex() const {
		return touchIndex;
	}

	float getGlobalX() const {
		return globalX;
	}

	float getGlobalY() const {
		return globalY;
	}

	float getLocalX() const {
		return localX;
	}

	float getLocalY() const {
		return localY;
	}

	void visitNode(IFlashDisplayObject* node);
private:
	int touchIndex;
	float globalX, globalY;
	float localX, localY;
};

class PinchEvent: public FlashEvent{
public:
	PinchEvent(const std::string& type, float globalX, float globalY, float scale);

	float getGlobalX() const {
		return globalX;
	}

	float getGlobalY() const {
		return globalY;
	}

	float getLocalX() const {
		return localX;
	}

	float getLocalY() const {
		return localY;
	}

	float getScale() const {
		return scale;
	}

	void visitNode(IFlashDisplayObject* node);

private:
	int touchIndex;
	float globalX, globalY;
	float localX, localY;
	float scale;
};