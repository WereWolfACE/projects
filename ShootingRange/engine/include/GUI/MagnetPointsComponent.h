#pragma once

#include "Utils/FPoint.h"
#include "GUI/ScrollableFrameModel.h"
#include "GUI/AutoScrollComponent.h"

namespace GUI {

//
// Компонент определяющий прокрутку фрейма с остановками на фиксированных точках
//
class MagnetPointsComponent {
public:
	void Init(ScrollableFrameModel* model, AutoScrollComponent* autoScroll);

	void CorrectTarget(const FPoint& lastVelocity, const FPoint& shiftStart);

	void SetStepWidth(float width) { _step.x = width; }
	void SetStepHeight(float height) { _step.y = height; }
	float GetStepWidth() const { return _step.x; }
	float GetStepHeight() const { return _step.y; }
	void SetColumnsCount(int columnsCount);
	void SetRowsCount(int rowsCount);
	int GetColumnsCount() const;
	int GetRowsCount() const;
	int GetCurrentColumn() const;
	int GetCurrentRow() const;

	void SetTargetColumn(int column) { SetTargetColumn(column, AutoScrollComponent::TimeConstants::normal); }
	void SetTargetColumn(int column, float curTimeConstant);
	void SetTargetRow(int row) { SetTargetRow(row, AutoScrollComponent::TimeConstants::normal); }
	void SetTargetRow(int row, float curTimeConstant);
	int GetTargetColumn() const;
	int GetTargetRow() const;

	FPoint GetPoint(int column, int row) const;

	void TurnOn() { _active = true; }
	void TurnOff() { _active = false; }

	bool HaveColumns() const { return _step.x > 0.f; }
	bool HaveRows() const { return _step.y > 0.f; }

private:
	int GetColumn(float x) const { return (int)math::round(-x / _step.x); }
	int GetRow(float y) const { return (int)math::round(-y / _step.y); }
	float GetX(int column) const { return -column * _step.x; }
	float GetY(int row) const { return -row * _step.y; }

	ScrollableFrameModel* _model;
	AutoScrollComponent* _autoScroll;

	FPoint _step;
	bool _active;
};

} // namespace GUI