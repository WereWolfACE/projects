#pragma once

#include "GUI/ScrollBarView.h"

namespace GUI {

class FunctorScrollBarAdapter : public IScrollBar {
public:
	typedef std::function<float()> GetValueFunctor;

	virtual float GetFirstValue() const override { Assert(_fntrFirstValue); return _fntrFirstValue ? _fntrFirstValue() : 0.0f; }
	virtual float GetLastValue() const override { Assert(_fntrLastValue); return _fntrLastValue ? _fntrLastValue() : 0.0f; }
	virtual float GetCurValue() const override { Assert(_fntrCurValue); return _fntrCurValue ? _fntrCurValue() : 0.0f; }

	void SetFirstValueFunctor(GetValueFunctor fntrFirstValue) { _fntrFirstValue = fntrFirstValue; }
	void SetLastValueFunctor(GetValueFunctor fntrLastValue) { _fntrLastValue = fntrLastValue; }
	void SetCurValueFunctor(GetValueFunctor fntrCurValue) { _fntrCurValue = fntrCurValue; }

	void SetFunctors(GetValueFunctor first, GetValueFunctor last, GetValueFunctor current) {
		_fntrFirstValue = first;
		_fntrLastValue = last;
		_fntrCurValue = current;
	}

private:
	GetValueFunctor _fntrFirstValue;
	GetValueFunctor _fntrLastValue;
	GetValueFunctor _fntrCurValue;
};

} // namespace GUI
