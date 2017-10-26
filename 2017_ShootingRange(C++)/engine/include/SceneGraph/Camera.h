#ifndef __CAMERA_H__
#define __CAMERA_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "Utils/Vector3.h"
#include "Utils/Matrix4.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Камера.
///
/// Инкапсулирует преобразования проекции и вида, а также позволяет
/// изменять положение вида в пространстве (анимировать камеру).
///
class Camera : public RefCounter {
public:
	typedef boost::intrusive_ptr<Camera> HardPtr;
	
	Camera(const math::Vector3& eye = math::Vector3(0.0f, 0.0f, 0.0f), const math::Vector3& lookAt = math::Vector3(0.0f, 0.0f, 1.0f));
	
	virtual ~Camera();
	
	virtual Camera::HardPtr Clone();

	virtual size_t GetMemoryInUse() const;
	
	const math::Matrix4& GetView();
		/// Возвращает матрицу вида
	
	const math::Matrix4& GetProjection() const;
		/// Возвращает матрицу проекции
	
	const math::Matrix4& GetViewProjection();
		/// Возвращает комбинированную матрицу вида-проекции
	
	const math::Vector3& GetRight() const;
		/// Возвращает правый вектор камеры
	
	const math::Vector3& GetUp() const;
		/// Возвращает верхний вектор камеры
	
	const math::Vector3& GetLookAt() const;
		/// Возвращает вектор направления взгляда камеры
	
	const math::Vector3& GetEye() const;
		/// Возвращает позицию камеры в мировом пространстве
	
	virtual void SetProjection(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
		/// Устанавливает параметры проецирования
	
	virtual void SetView(const math::Vector3& eye, const math::Vector3& lookAt);
		/// Устанавливает параметры вида камеры
	
	virtual void Yaw(float radians);
		/// Поворачивает камеру относительно верхнего вектора (рыскание)
	
	virtual void Pitch(float radians);
		/// Поворачивает камеру относительно правого вектора (наклон)
	
	virtual void Roll(float radians);
		/// Поворачивает камеру относительно вектора взгляда (вращение)
	
	virtual void Walk(float units);
		/// Передвигает камеру относительно вектора взгляда (вперёд-назад)
	
	virtual void Strafe(float units);
		/// Передвигает камеру относительно правого вектора (вправо-влево)
	
	virtual void Fly(float units);
		/// Передвигает камеру относительно верхнего вектора (вверх-вниз)

protected:
	void RecalculateView();
		/// Пересчитывает матрицы вида и вида-проекции после изменения векторов камеры

protected:
	math::Vector3 _right;
		/// Правый вектор камеры
	
	math::Vector3 _up;
		/// Верхний вектор камеры
	
	math::Vector3 _lookAt;
		/// Вектор взгляда камеры
	
	math::Vector3 _eye;
		/// Положение наблюдателя в мировом пространстве
	
	math::Matrix4 _view;
		/// Матрица вида, управляемая камерой
	
	math::Matrix4 _proj;
		/// Матрица проекции, управляемая камерой
	
	math::Matrix4 _viewProj;
		/// Комбинированная матрица вида-проекции
	
	bool _viewDirty;
		/// Нужно ли пересчитать матрицы вида и вида-проекции
	
	float _fieldOfView;
		/// Угол зрения камеры
	
	float _aspectRatio;
		/// Форматное соотношение проецирования
	
	float _nearPlane;
		/// Плоскость ближнего отсечения
	
	float _farPlane;
		/// Плоскость дальнего отсечения
};

} // namespace SceneGraph

#endif // __CAMERA_H__
