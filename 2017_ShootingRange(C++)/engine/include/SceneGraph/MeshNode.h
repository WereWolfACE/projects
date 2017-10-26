#ifndef __MESHNODE_H__
#define __MESHNODE_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include <boost/intrusive_ptr.hpp>

#include "SceneNode.h"
#include "IMesh.h"

namespace SceneGraph
{

///
/// Узел сцены, визуализирующий набор мешей.
///
class MeshNode : public SceneNode {
public:
	typedef boost::intrusive_ptr<MeshNode> HardPtr;
	typedef std::vector<IMeshPtr> Meshes;

public:
	MeshNode(const std::string& name, SceneNode* parent, Scene* scene);

public:
	SceneNode* Clone();
	size_t GetMemoryInUse() const;
	
	unsigned int GetRenderPassMask() const;
		/// Возвращает типы проходов визуализации, при которых должна производиться отрисовка узла

	void AddMesh(IMesh* mesh);
		/// Добавляет меш к коллекции мешей узла
	
	const Meshes& GetMeshes() const;
		/// Возвращает коллекцию мешей узла
	
	void Draw() const;
		/// Вызывается каждый фрейм для отрисовки узла

	virtual bool Hit(const math::Vector3 &base, const math::Vector3 &dir) const;
		/// проверка подадания в меши данного узла

protected:
	MeshNode(const MeshNode& rhs);
	
	MeshNode& operator = (const MeshNode& rhs);

protected:
	Meshes _meshes;
		/// Набор мешей
};

} // namespace SceneGraph

#endif // __MESHNODE_H__
