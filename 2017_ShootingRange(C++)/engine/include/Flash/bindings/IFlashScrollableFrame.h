#pragma once

#include "../core/IFlashSprite.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif


//-----------------------------------------------------------------------------
struct IFlashScrollableFrame : public IFlashSprite
{
	/**
	* ������������� � ��������� frame rect
	*/
	virtual void init(int left, int top, int right, int bottom) = 0;

	/**
	* ���������/���������� �������������/��������������� �������.
	* �������� false ��������, ��� �� ����� �������� ������ ������� � ���������� � ������ �����������,
	* �� ����� ��������� �������� resetScroll.
	*/
	virtual void enableScroll(bool horizontal, bool vertical) = 0;

	/**
	* ������ ������� �������������� ������
	*/
	virtual void setFrameRect(int left, int top, int right, int bottom) = 0;
	virtual void getFrameRect(int& left, int& top, int& right, int& bottom) = 0;

	/**
	* �������� ����������� ��������� ��������� �����
	*/
	virtual void enableMouseWheel(bool isEnabled) = 0;

	/**
	* ������ ���������� ������� ��������
	*/
	virtual void setMouseWheelVertical(bool isVertical) = 0;
	virtual bool isMouseWheelVertical() const = 0;

	/**
	* ������� ��������� �������.
	* scroll ��������� � �������� �� getMinScroll() �� getMaxScroll()
	*/
	virtual void getScroll(float& x, float& y) = 0;
	virtual void getMinScroll(float& x, float& y) = 0;
	virtual void getMaxScroll(float& x, float& y) = 0;

	/**
	* ���������� ������� � ������� �������� ��������� � ��������� ��������
	* ������������� ����� ����������.
	*/
	virtual void resetScroll(float x, float y) = 0;
	virtual void resetScroll(float x, float y, float minusLength) = 0;

	virtual bool inManualScroll() const = 0;

	virtual void setAutoScrollScaleFactor(float scaleFactor) = 0;
	virtual void setAutoScrollTimeConstant(float timeConstant) = 0;

	virtual void activateAutoScroll() = 0;
	virtual void deactivateAutoScroll() = 0;

	virtual void setTarget(float targetX, float targetY) = 0;
	virtual void setTarget(float targetX, float targetY, float curTimeConstant) = 0;
	virtual void getTarget(float& targetX, float& targetY) const = 0;

	virtual bool needHorizontalScroll() const = 0;
	virtual bool needVerticalScroll() const = 0;
	virtual bool needScrollLeft() const = 0;
	virtual bool needScrollRight() const = 0;
	virtual bool needScrollDown() const = 0;
	virtual bool needScrollUp() const = 0;

	virtual void setMagnetStepWidth(float width) = 0;
	virtual void setMagnetStepHeight(float height) = 0;
	virtual float getMagnetStepWidth() const = 0;
	virtual float getMagnetStepHeight() const = 0;
	virtual void setColumnsCount(int columnsCount) = 0;
	virtual void setRowsCount(int rowsCount) = 0;
	virtual int getColumnsCount() = 0;
	virtual int getRowsCount() = 0;
	virtual int getCurrentColumn() = 0;
	virtual int getCurrentRow() = 0;

	virtual void setTargetColumn(int column) = 0;
	virtual void setTargetColumn(int column, float curTimeConstant) = 0;
	virtual void setTargetRow(int row) = 0;
	virtual void setTargetRow(int row, float curTimeConstant) = 0;

	virtual int getTargetColumn() const = 0;
	virtual int getTargetRow() const = 0;

	virtual void resetMagnetPoint(int column, int row, float minusLength) = 0;

	virtual void magnetTurnOn() = 0;
	virtual void magnetTurnOff() = 0;

	virtual bool isMoving() const = 0;
	virtual bool isAutoscrollMoving() const = 0;

	/**
	* ���������, ��� ������� ��� ��������� ������-���� �������� ���������� ����������.
	*/
	virtual void contentChanged() = 0;

	/**
	* ������������� ������ ����������� ���������� (����� ������� ����� �������� ��� ���������)
	*/
	virtual void setContentRect(float left, float top, float right, float bottom) = 0;
	virtual void getContentRect(float& left, float& top, float& right, float& bottom) = 0;

	/**
	* ������� �� ������� �������� (ContentRect).
	* ������ � ���������� ContentRect �������� �������� ������� ���������.
	* ������������ ��� ������� �������������� �������� ��� autoContentRect.
	*/
	virtual void setContentInsets(float left, float top, float right, float bottom) = 0;
	virtual void getContentInsets(float& left, float& top, float& right, float& bottom) = 0;

	/**
	* ���� ��������, ������ ������� ��� ��������� ����� ������������� ����������� �� ������� �����������.
	*/
	virtual void enableAutoContentRect(bool isEnabled) = 0;
	virtual bool isAutoContentRect() = 0;

	/**
	* ������ ��������� ��������, ����� ������� ������ ������� ��������� (�� ��������� ����� ������� ����)
	* (0.f, 0.f) - ����� ������� ����, (0.5f, 0.5f) �����, (1.f, 1.f) - ������ ������ ���� � �.�.
	*/
	virtual void setContentAlign(float hAlign, float vAlign) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
