#pragma once

#include "GUI/ScrollableFrameModel.h"
#include "GUI/ManualScrollComponent.h"
#include "GUI/AutoScrollComponent.h"
#include "GUI/MagnetPointsComponent.h"

namespace GUI {

enum class MouseWheelOrientation {
	Vertical,
	Horizontal
};

//
// Прокручиваемый фрейм
//
class ScrollableFrame : public RefCounter {
public:
	ScrollableFrame() = default;

	void AddManualScrollOnEndHandler(ManualScrollComponent::OnEndHandler handler) { _manualScroll.AddOnEndHandler(handler); }
	void Init(const IRect& rectFrame);

	void Update(float dt);

	bool MouseDown(const IPoint& mouse_pos) { return _manualScroll.MouseDown(mouse_pos); }
	void MouseUp(const IPoint& mouse_pos) { _manualScroll.MouseUp(mouse_pos); }
	void MouseMove(const IPoint& mouse_pos) { _manualScroll.MouseMove(mouse_pos); }
	void MouseWheel(int delta);

	void SetMouseWheelOrientation(MouseWheelOrientation mouseWheelOrientation) { _mouseWheelOrientation = mouseWheelOrientation; }
	MouseWheelOrientation GetMouseWheelOrientation() const { return _mouseWheelOrientation; }

	void SetFrameRect(const IRect& rectFrame) { _model.SetFrameRect(rectFrame); }
	const IRect& GetFrameRect() const { return _model.GetFrameRect(); }
	void MoveTo(const IPoint& point) { _model.MoveTo(point); }

	const FPoint& GetContentShift() const { return _model.GetContentShift(); }
	FPoint GetContentShiftLimit() const { return _model.GetContentShiftLimit(); }
	void SetContentWidth(float widthContent) { _model.SetContentWidth(widthContent); }
	void SetContentHeight(float heightContent) { _model.SetContentHeight(heightContent); }
	float GetContentWidth() const { return _model.GetContentWidth(); }
	float GetContentHeight() const { return _model.GetContentHeight(); }

	// Установить текущее смещение прокри, и целевое смещение прокрутки в указанное значение
	void ResetContentShift(const FPoint& contentShift, float minusLength = 0.0f);

	bool HitTest(const IPoint& point) const { return _model.HitTest(point); }
	bool InManualScroll() const { return _manualScroll.InScroll(); }

	void SetAutoScrollScaleFactor(float scaleFactor) { _autoScroll.SetScaleFactor(scaleFactor); }
	void SetAutoScrollTimeConstant(float timeConstant) { _autoScroll.SetTimeConstant(timeConstant); }

	void ActivateAutoScroll() { _autoScroll.Activate(); }
	void DeactivateAutoScroll() { _autoScroll.Deactivate(); }

	void SetTarget(const FPoint& target) { _autoScroll.SetTarget(target); }
	void SetTarget(const FPoint& target, float curTimeConstant) { _autoScroll.SetTarget(target, curTimeConstant); }
	const FPoint& GetTarget() const { return _autoScroll.GetTarget(); }

	bool NeedHorizontalScroll() const { return _model.NeedHorizontalScroll(); }
	bool NeedVerticalScroll() const { return _model.NeedVerticalScroll(); }
	bool NeedScrollLeft() const { return _model.NeedScrollLeft(); }
	bool NeedScrollRight() const { return _model.NeedScrollRight(); }
	bool NeedScrollDown() const { return _model.NeedScrollDown(); }
	bool NeedScrollUp() const { return _model.NeedScrollUp(); }

	void SetMagnetStepWidth(float width) { _magnetPoints.SetStepWidth(width); }
	void SetMagnetStepHeight(float height) { _magnetPoints.SetStepHeight(height); }
	float GetMagnetStepWidth() const { return _magnetPoints.GetStepWidth(); }
	float GetMagnetStepHeight() const { return _magnetPoints.GetStepHeight(); }
	void SetColumnsCount(int columnsCount) { _magnetPoints.SetColumnsCount(columnsCount); }
	void SetRowsCount(int rowsCount) { _magnetPoints.SetRowsCount(rowsCount); }
	int GetColumnsCount() const { return _magnetPoints.GetColumnsCount(); }
	int GetRowsCount() const { return _magnetPoints.GetRowsCount(); }
	int GetCurrentColumn() const { return _magnetPoints.GetCurrentColumn(); }
	int GetCurrentRow() const { return _magnetPoints.GetCurrentRow(); }

	void SetTargetColumn(int column) { _magnetPoints.SetTargetColumn(column); }
	void SetTargetColumn(int column, float curTimeConstant) { _magnetPoints.SetTargetColumn(column, curTimeConstant); }
	void SetTargetRow(int row) { _magnetPoints.SetTargetRow(row); }
	void SetTargetRow(int row, float curTimeConstant) { _magnetPoints.SetTargetRow(row, curTimeConstant); }

	int GetTargetColumn() const { return _magnetPoints.GetTargetColumn(); }
	int GetTargetRow() const { return _magnetPoints.GetTargetRow(); }

	void ResetMagnetPoint(int column, int row, float minusLength = 0.0f);

	void MagnetTurnOn() { _magnetPoints.TurnOn(); }
	void MagnetTurnOff() { _magnetPoints.TurnOff(); }

	bool IsMoving() const;
	bool IsAutoscrollMoving() const { return _autoScroll.IsMoving(); }

	const IScrollBar* GetHorizontalScrollBar() const { return _model.GetHorizontalScrollBar(); }
	const IScrollBar* GetVerticalScrollBar() const { return _model.GetVerticalScrollBar(); }

private:
	ScrollableFrameModel _model;
	ManualScrollComponent _manualScroll;
	AutoScrollComponent _autoScroll;
	MagnetPointsComponent _magnetPoints;
	MouseWheelOrientation _mouseWheelOrientation;
};

typedef boost::intrusive_ptr<ScrollableFrame> ScrollableFramePtr;

} // namespace GUI
