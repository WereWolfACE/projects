#pragma once

#include <RefCounter.h>
#include <Utils/Vector3.h>
#include <Utils/Matrix4.h>

#include "AxisAlignedBoundingBox.h"
#include "Material.h"

namespace SceneGraph
{

struct IMesh : public RefCounter
{
	virtual void LoadData() = 0;
	virtual void UnloadData() = 0;

	virtual IMesh* Clone() = 0;
	virtual size_t GetMemoryInUse() const = 0;

	virtual bool IsSkeleton() const = 0;
	virtual MaterialPtr GetMaterial() const = 0;
	virtual void Draw() = 0; /// ќтрисовывает себ€
	virtual bool Hit(const math::Vector3 &base, const math::Vector3 &dir) const = 0; /// ѕроверка попадани€ в меш
	virtual AxisAlignedBoundingBox GetAABBox(const math::Matrix4& _view) const = 0; /// ¬озвращает ограничивающий параллелепипед, выровненный по ос€м координат(https://ru.wikipedia.org/wiki/AABB)
};

typedef boost::intrusive_ptr<IMesh> IMeshPtr;
};
