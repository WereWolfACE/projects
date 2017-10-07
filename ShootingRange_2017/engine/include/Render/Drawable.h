#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#pragma once

#include "Utils/FPoint.h"
#include "Render/RenderTypes.h"

namespace Render {

class SpriteBatch;

///
/// Базовый класс для рисуемых сущностей.
///
class Drawable : public RefCounter {
public:
	virtual ~Drawable() { }

	virtual int Width() const = 0;

	virtual int Height() const = 0;

	virtual bool HitTest(int x, int y) const = 0;

	virtual void Draw(SpriteBatch* batch, const FPoint& position) = 0;

	virtual void GetGeometry(std::vector<QuadVert>& geometry, float z = 0, Color color = Color::WHITE) const = 0;
};

typedef boost::intrusive_ptr<Drawable> DrawablePtr;

} // namespace Render

#endif // __DRAWABLE_H__
