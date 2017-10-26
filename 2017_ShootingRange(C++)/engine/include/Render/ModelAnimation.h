#ifndef _RENDER_MODELANIMATION_H_
#define _RENDER_MODELANIMATION_H_

#include "SceneGraph.h"
#include "Core/Resource.h"

class ModelAnimationLoader;

namespace Render {

typedef boost::intrusive_ptr<class ModelAnimation> ModelAnimationPtr;

///
/// Класс анимации, воспроизводящей анимированные 3D модели.
///
class ModelAnimation : public Resource {
public:
	explicit ModelAnimation(rapidxml::xml_node<>* elem);

	ModelAnimation(const ModelAnimation& rhs);
		// Конструктор копий

	virtual ModelAnimation* Clone() const;

	const std::string& GetName() const { return _name; }

	size_t GetMemoryInUse() const;

	SceneGraph::ModelNode::HardPtr GetModel() const { return _model; }

	void MoveTo(const FPoint& position);

	void SetScale(float s);

	void Update(float dt);

	void Draw();

	// Создаёт экземпляр анимации
	static ModelAnimationPtr Spawn(const std::string& id);

private:
	friend class ::ModelAnimationLoader;

	ModelAnimation& operator=(const ModelAnimation&);
		// Присваивание запрещено

	void LoadFromFile(const std::string& filename);
		// Загружает модель из файла

	static math::Vector3 ExtractVector3(rapidxml::xml_node<>* elem);

private:
	std::string _path;
		// Путь к файлу модели

	std::string _name;
		// Идентификатор анимации

	SceneGraph::ModelNode::HardPtr _model;
		// Отображаемая модель

	math::Matrix4 _originalTransform;
		// Преобразование из описания ресурса

	FPoint _position;

	float _scale = 1.0f;
};

} // namespace Render

#endif // _RENDER_MODELANIMATION_H_
