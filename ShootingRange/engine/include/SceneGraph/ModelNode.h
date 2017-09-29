#ifndef __MODELNODE_H__
#define __MODELNODE_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include <list>

#include "SceneGraph/SceneNode.h"
#include "SceneGraph/Animation.h"

namespace SceneGraph {

class LazyKeyFramedAnimationPool;
///
/// Модель, которая может состоять из нескольких мешей и скелета.
/// Содержит набор анимаций, которые могут комбинироваться друг с другом.
///
class ModelNode : public SceneNode {
public:
	typedef boost::intrusive_ptr<ModelNode> HardPtr;
	
	typedef std::map<std::string, Animation::HardPtr> Animations;

	ModelNode(const std::string& name, SceneNode* parent, Scene* scene);
	
	ModelNode* CloneModel() const;	
	SceneNode* Clone();

	size_t GetMemoryInUse() const;
	
	unsigned int GetRenderPassMask() const;
		/// Возвращает типы проходов визуализации, при которых должна производиться отрисовка узла
	
	SceneNode::HardPtr FindFrame(const std::string& name) const;
		/// Рекурсивно ищет узел среди фреймов
	
	inline void AddMeshNode(const SceneNode::HardPtr& _meshNode){ __m_meshNodes.push_back(_meshNode); } /// Добавляет meshНоду
	inline const SceneNodesList& getMeshNodes(){ return __m_meshNodes; }; ///Возвражает список нод в модели содержащих меши
	
	const Animations& GetAnimations() const;
		/// Возвращает коллекцию анимаций
	
	Animation::HardPtr GetAnimation(const std::string& name) const;
		/// Находит анимацию по имени
	
	void AddAnimation(Animation::HardPtr animation);
		/// Добавляет анимацию в коллекцию

	void SetAnimations(const Animations& animations);
		/// Подключает адаптированную копию коллекции анимаций

	void Update(float dt);
		/// Реакция на течение времени
	
	void Draw() const;
		/// Рисует модель
	
	void ShowSkeleton(bool show);
		/// Отображать ли скелет
	
	void ShowBoundingBox(bool show);
		/// Отображать ли ограничивающий прямоугольник

	bool HitBoundingBox(const IPoint &mouse_pos) const;
		/// Пересекает ли указанный луч из курсора BoundingBox модели 

	bool HitModel(const IPoint &mouse_pos) const;
		/// Пересекает ли указанный луч из курсора модель

	SceneNode* FindFrameInAllFrames(const std::string& name) const;
		/// ищем фрейм в массиве всех фреймов

	const boost::intrusive_ptr<LazyKeyFramedAnimationPool>& getLazyAnimationsPool();
	inline uint32_t getLazyAnimationsPoolSize() const { return __m_poolSize; };
	void setLazyAnimationsPoolSize(uint32_t _poolSize);

	virtual void acceptScene(const Scene* _scene);

private:
	void __FillAllFrames();

protected:
	ModelNode(const ModelNode& rhs);
	
	ModelNode& operator = (const ModelNode& rhs);
	
	void DrawSkeleton() const;
		/// Рисует скелет
		
	void DrawBoundingBox() const;
		/// Рисует ограничивающий прямоугольник

protected:
	SceneNodesList __m_meshNodes;

	SceneNodesMap _allFrames; /// Собираем все фреймы в один общий массив для более быстрого поиска фрейма по имени	
	SceneNodesList _allFramesList; /// Плоский список фреймов для последовательного обновления

	Animations _animations;
	boost::intrusive_ptr<LazyKeyFramedAnimationPool> __m_pool;
	uint32_t __m_poolSize;
		/// Набор анимаций модели.
		/// Модель может состоять из нескольких мешей в иерархии фреймов.
		/// Каждая анимация может включать в себя трансформации нескольких фреймов.
	
	bool _showSkeleton;
		/// Отображать ли скелет
	
	bool _showBoundingBox;
		/// Отображать ли ограничивающий прямоугольник
};

} // namespace SceneGraph

#endif // __MODELNODE_H__
