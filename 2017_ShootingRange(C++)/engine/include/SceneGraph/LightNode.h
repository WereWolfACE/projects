#ifndef __LIGHTNODE_H__
#define __LIGHTNODE_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "SceneNode.h"
#include "Light.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Узел сцены с источником освещения.
///
class LightNode : public SceneNode {
public:
	typedef boost::intrusive_ptr<LightNode> HardPtr;
	
	LightNode(const std::string& name, SceneNode* parent, Scene* scene);
	
	LightNode(const LightNode& rhs);
	
	LightNode& operator = (const LightNode& rhs);
	
	SceneNode* Clone();

	size_t GetMemoryInUse() const;
	
	unsigned int GetRenderPassMask() const;
		/// Возвращает типы проходов визуализации, при которых должна производиться отрисовка узла
	
	Light::HardPtr GetLight() const;
		/// Возвращает источник света
	
	void SetLight(Light::HardPtr light);
		/// Устанавливает источник света
	
	void Update(float dt);
		/// Вызывается каждый фрейм для реакции узла на течение времени
	
	void Draw() const;
		/// Вызывается каждый фрейм для отрисовки узла

protected:
	Light::HardPtr _light;
		/// Источник света
};

} // namespace SceneGraph

#endif // __LIGHTNODE_H__
