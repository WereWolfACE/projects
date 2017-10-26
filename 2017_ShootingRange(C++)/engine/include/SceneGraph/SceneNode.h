#ifndef __SCENENODE_H__
#define __SCENENODE_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "SceneGraph/AxisAlignedBoundingBox.h"
#include "SceneGraph/SceneNode.h"
#include "Utils/Matrix4.h"
#include "Utils/Quaternion.h"
#include "Render/Target.h"
#include "RefCounter.h"

#include <map>

namespace SceneGraph {

class Scene;
class ModelNode;

///
/// Базовый класс для всех узлов сцены.
///
class SceneNode : public RefCounter {
public:
	typedef boost::intrusive_ptr<SceneNode> HardPtr;

	typedef std::map<std::string, SceneNode::HardPtr> SceneNodesMap;
	typedef std::list<SceneNode::HardPtr> SceneNodesList;

	///
	/// Перечисляет типы узлов сцены.
	///
	enum Type {
		Unknown,
			/// Неизвестный узел (не отображается)
		Frame,
			/// Фрейм
		Mesh,
			/// Меш
		Model,
			/// Модель
		Light,
			/// Источник света
		Camera
			/// Камера
	};

	SceneNode(const std::string& name, SceneNode* parent, Scene* scene, SceneNode::Type type = SceneNode::Unknown);

	virtual SceneNode* Clone();

	virtual size_t GetMemoryInUse() const;

	SceneNode::Type GetType() const;
		/// Возвращает тип узла сцены

	void SetType(SceneNode::Type type);
		/// Устанавливает тип узла сцены

	virtual unsigned int GetRenderPassMask() const;
		/// Возвращает типы проходов визуализации, при которых должна производиться отрисовка узла

	const std::string& GetName() const;
		/// Возвращает имя узла

	void SetName(const std::string& name);
		/// Устанавливает имя узла

	SceneNode* GetParent() const;
		/// Возвращает родительский узел

	void SetParent(SceneNode* parent);
		/// Устанавливает родительский узел

	Scene* GetScene() const;
		/// Возвращает сцену, которой принадлежит данный узел

	void SetScene(Scene* scene);
		/// Устанавливает сцену, которой принадлежит данный узел

	const math::Vector3& GetOriginalPosition() const;
	const math::Quaternion& GetOriginalRotation() const;
	const math::Vector3& GetOriginalScaling() const;
		/// Возвращают исходные данные трансформации

	const math::Matrix4& GetOriginalTransform() const;
		/// Возвращает матрицу исходного преобразования узла

	void SetOriginalTransform(const math::Matrix4& transform);
		/// Устанавливает матрицу исходного преобразования узла

	void MultiplyOriginalTransform(const math::Matrix4& transform);
		/// Умножает матрицу исходного преобразования узла на указанную матрицу трансформации

	void MultiplyOriginalTransformLocal(const math::Matrix4& transform);
		/// Локально умножает матрицу исходного преобразования узла на указанную матрицу трансформации

	inline void AllFramesResetRelativeTransform(){_relativeTransform = _originalTransform;} /// Устанавливает матрицу относительного преобразования матрице исходного преобразования

	const math::Matrix4& GetRelativeTransform() const;
		/// Возвращает матрицу относительного преобразования узла

	void SetRelativeTransform(const math::Matrix4& transform);
		/// Устанавливает относительного преобразования узла

	void SetAbsoluteTransform(const math::Matrix4& transform);
		/// Устанавливает абсолютное преобразование узла

	void AddRelativeTransform(const math::Matrix4& transform);
		/// Добавляет преобразование к относительному преобразованию узла

	void MultiplyRelativeTransform(const math::Matrix4& transform);
		/// Умножает относительное преобразование узла на указанную матрицу трансформации

	void MultiplyRelativeTransformLocal(const math::Matrix4& transform);
		/// Локально умножает относительное преобразование узла на указанную матрицу трансформации

	void UpdateAbsoluteTransform();
		/// Рекурсивно вычисляет трансформацию узла относительно мировой системы координат

	const math::Matrix4& GetAbsoluteTransform() const;
		/// Возвращает матрицу абсолютного преобразования узла

	const AxisAlignedBoundingBox& GetBoundingBox() const;
		/// Возвращает выровненный по осям ограничивающий прямоугольник

	AxisAlignedBoundingBox GetTransformedBoundingBox() const;
		/// Возвращает трансформированный ограничивающий прямоугольник

	void SetBoundingBox(const AxisAlignedBoundingBox& aabbox);
		/// Устанавливает ограничивающий прямоугольник, выровненный по осям

	const SceneNodesMap& GetChildren() const; /// Возвращает коллекцию дочерних узлов

	bool AddChild(SceneNode::HardPtr node);
		/// Добавляет узел к списку дочерних узлов

	SceneNode::HardPtr RemoveChild(const std::string& name);
		/// Удаляет узел из списка дочерних узлов нерекурсивно

	void Remove();
		/// Удаляет себя из списка дочерних узлов родителя

	SceneNode::HardPtr FindChild(const std::string& name) const;
		/// Рекурсивно ищет узел среди детей

	virtual void Update(float dt);
		/// Вызывается каждый фрейм для реакции узла на течение времени

	virtual void Draw() const;/// Вызывается каждый фрейм для отрисовки узла

	virtual bool Hit(const math::Vector3 &base, const math::Vector3 &dir) const;
		/// проверка попадания в объект, актуально для мешей

	virtual void acceptScene(const Scene* _scene);
protected:
	SceneNode(const SceneNode& rhs);

	SceneNode& operator = (const SceneNode& rhs);

protected:
	SceneNode::Type _type = SceneNode::Unknown;
		/// Тип узла сцены

	std::string _name;
		/// Уникальное имя узла

	Scene* _scene = nullptr;
		/// Сцена, которой принадлежит данный узел

	SceneNode* _parent = nullptr;
		/// Родительский узел

	SceneNodesMap _children;
		/// Отображение имени на узел для быстрого поиска детей

		///
		/// Преобразование OriginalTransform являет собой относительное преобразование узла
		/// по отношению к преобразованию родительского узла, остаётся неизменным во время
		/// анимирования и устанавливается во время загрузки модели.
		///
		/// Преобразование RelativeTransform устанавливается равным OriginalTransform
		/// в начале каждого кадра и дополняется преобразованиями во время наложения анимаций.

	math::Vector3 _originalPosition;
	math::Quaternion _originalRotation;
	math::Vector3 _originalScaling;
		/// Исходные преобразования трансформации (вычисляются на основе декомпозиции _originalTransform
		/// Нужны для установки данных трансформации по-умолчанию при KeyframeAnimation

	math::Matrix4 _originalTransform; /// Исходная трансформация узла.
	math::Matrix4 _relativeTransform; /// Трансформация узла относительно пространства родительского узла.
	math::Matrix4 _absoluteTransform; /// Трансформация относительно мирового пространства.
		///
		/// Преобразование AbsoluteTransform является абсолютным по отношению к миру
		/// и рассчитывается для всей иерархии одновременно после того,
		/// как будут произведены все изменения в относительном преобразовании всех узлов сцены.

	AxisAlignedBoundingBox _aabbox;
		/// Ограничивающий прямоугольник, выровненный по осям
};

} // namespace SceneGraph

#endif // __SCENENODE_H__
