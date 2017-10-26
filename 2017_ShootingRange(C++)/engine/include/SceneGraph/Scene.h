#ifndef __SCENE_H__
#define __SCENE_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "SceneGraph/SceneNode.h"
#include "SceneGraph/CameraNode.h"
#include "SceneGraph/LightNode.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Типы проходов визуализации.
///
enum class RenderPass {
	Unknown = 0,
	Camera = 1,
	Light = 2,
	Opaque = 4,
	Transparent = 8,
	TransparentEffect = 16
};

///
/// Сцена.
///
class Scene : public RefCounter {
public:
	typedef boost::intrusive_ptr<Scene> HardPtr;
	
	Scene();
	
	virtual ~Scene();
	
	SceneNode* GetRootNode();
		/// Возвращает корневой узел сцены
	
	SceneNode::HardPtr AddSceneNode(SceneNode::HardPtr node
									, const math::Vector3& position = math::Vector3(0.0f, 0.0f, 0.0f)
									, const math::Vector3& rotation = math::Vector3(0.0f, 0.0f, 0.0f) // pitch, yaw, roll
									, const math::Vector3& scale = math::Vector3(1.0f, 1.0f, 1.0f));
		/// Добавляет новый узел в сцену
	
	CameraNode::HardPtr AddCamera(Camera::HardPtr camera);
		/// Добавляет камеру в сцену
	
	LightNode::HardPtr AddLight(Light::HardPtr light);
		/// Добавляет источник света в сцену
	
	Camera::HardPtr GetActiveCamera() const;
		/// Возвращает активную камеру
	
	void SetActiveCamera(Camera::HardPtr camera);
		/// Устанавливает активную камеру
	
	RenderPass GetCurrentRenderPass() const;
		/// Возвращает текущий проход визуализации
	
	virtual void Update(float dt);
		/// Обновляет состояние сцены

	virtual void Draw();
	void Draw(SceneNode*) const; /// отрисовка ноды сценой(по сути дела это инплементация поттерна visitor)
	void Draw(ModelNode*) const; /// отрисовка модели сценой

protected:
	SceneNode::HardPtr _root;
		/// Корневой узел сцены
	
	Camera::HardPtr _activeCamera;
		/// Активная камера
	
	RenderPass _currentRenderPass;
		/// Текущий проход визуализации	
};

} // namespace SceneGraph

#endif // __SCENE_H__
