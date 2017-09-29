#ifndef __COLORVALUE_H__
#define __COLORVALUE_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

namespace SceneGraph {

///
/// Значение цвета, использующее нормализованные компоненты.
///
class ColorValue {
public:
	ColorValue();

	ColorValue(float r, float g, float b, float a = 1.0f);

	explicit ColorValue(unsigned int argb);

public:
	float r;
		/// Red

	float g;
		/// Green

	float b;
		/// Blue

	float a;
		/// Alpha
};

} // namespace SceneGraph

#endif // __COLORVALUE_H__
