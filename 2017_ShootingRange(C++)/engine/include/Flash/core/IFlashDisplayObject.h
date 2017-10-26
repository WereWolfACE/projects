#pragma once

#include <string>
#include <Utils/Color.h>

#include "IFlashUnknown.h"
#include "IHitTestDelegate.h"
#include "IFlashVisitor.h"

//---------------------------------------------------------------------------
class FlashRender;
class FlashUpdateListener;
struct IFlashCustomDrawOperation;
struct IFlashSprite;

//---------------------------------------------------------------------------
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

struct IFlashDisplayObject: public IFlashUnknown
{
	/**
	* Рисование объекта в заданный массив данных
	* За преобразование координат и применение цвета несет ответственность вызывающий.
	*/
	virtual void render(FlashRender& render) = 0;
	/**
	* Запускает процедуру проверки попадания на объекте.
	* @param x, y задают точку в системе координат данного объекта
	* @see IHitTestDelegate
	*/
	virtual bool hitTest(float x, float y, IHitTestDelegate* hitTestDelegate) = 0;
	/**
	* Получает координаты ограничивающего прямоугольника в указанной системе координат
	* @return true если объект имеет ограничивающий прямоугольник и false - если объект пуст
	*/
	virtual bool getBounds(float& left, float& top, float& right, float& bottom, IFlashDisplayObject* targetCoordinateSystem) = 0;

	/**
	* Применяет трансформацию another объекта так, будто бы он является родителем this
	*/
	virtual void applyTransform(IFlashDisplayObject* another) = 0;

	/**
	* Устанавливает объекту трансформацию прототипа, если он доступен
	* @return true если трансформация была установлена, false в противном случае
	*/
	virtual bool resetTransform() = 0;

	/**
	* Рекурсивное обновление объектов в иерархии
	*/
	virtual void advance(FlashUpdateListener* updateListener, float dt) = 0;

	/**
	* Устанавливает, является ли объект прозрачным для hitTest процедуры
	*/
	virtual void setHitTestTransparent(bool value) = 0;
	virtual bool getHitTestTransparent() const = 0;
	/**
	* Устанавливает, посылает ли объект попадание по нему при hitTest.
	* Свойство можно использовать, например, чтобы не получать информацию о
	* попадании во внутренний спрайт составного клипа. При этом сам объект
	* может быть непрозрачным, в таком случае о получении попадания сообщит его
	* родительский элемент
	*/
	virtual void setHitTestDispatcher(bool value) = 0;
	virtual bool getHitTestDispatcher() const = 0;

	/**
	* Устанавливает видимость объекта
	*/
	virtual void setVisible(bool value) = 0;
	/**
	* Возвращает флаг видимости объекта
	*/
	virtual bool getVisible() const = 0;

	/**
	* Устанавливает имя объекта
	*/
	virtual void setName(const std::string& name) = 0;

	/**
	* Возвращает имя объекта (instance name в adobe flash)
	*/
	virtual const std::string& getName() const = 0;

	/**
	* Устанавливает значение alpha для объекта
	*/
	virtual void setAlpha(float value) = 0;
	/**
	* Возвращает значение alpha для клипа
	*/
	virtual float getAlpha() const = 0;

	/**
	* Устанавливает цвет объекта
	*/
	virtual void setColor(const Color& color) = 0;

	/**
	* Возвращает цвет объекта
	*/
	virtual const Color& getColor() const = 0;

	/**
	* Устанавливает позицию относительно родительского объекта
	*/
	virtual void setPosition(float x, float y) = 0;
	/**
	 * Получает позицию точки привязки(это точка начало координат от которой все считается, положение этой точки задается в редакторе adobe Flash при экспорте в Action Script) относительно родительского объекта
	*/
	virtual void getPosition(float& x, float& y) = 0;
	/**
	* Устанавливает угол поворота в координатной системе родительского объекта
	*/
	virtual void setRotation(float radians) = 0;
	/**
	* Возвращает поворот в координатной системе родительского объекта
	*/
	virtual float getRotation() = 0;

	/**
	* Устанавливает искажение в координатной системе родительского объекта
	*/
	virtual void setShear(float k) = 0;
	/**
	* Возвращает искажение в координатной системе родительского объекта
	*/
	virtual float getShear() = 0;

	/**
	* Устанавливает масштаб в координатной системе родительского объекта
	*/
	virtual void setScale(float scaleX, float scaleY) = 0;
	/**
	* Получает масштаб в координатной системе родительского объекта
	*/
	virtual void getScale(float& x, float& y) = 0;

	/**
	* Устанавливает матрицу трансформации объекта
	* Должно быть предоставлено 6 значений:
	* a c tx
	* b d ty
	*/
	virtual void setMatrix(const float *matrix) = 0;
	/**
	* Возвращает матрицу трансформации объекта
	* Будет записано 6 значений:
	* a c tx
	* b d ty
	*/
	virtual void getMatrix(float *matrix) const = 0;

	/**
	* Производит преобразование точки в локальной системе координат
	* в систему координат родителя
	*/
	virtual void localToParent(float& x, float& y) const = 0;
	/**
	* Производит преобразование точки в системе координат родителя в
	* локальную систему координат
	*/
	virtual void parentToLocal(float& x, float& y) const = 0;
	/**
	* Выполняет преобразование точки в локальной системе координат в глобальную
	*/
	virtual void localToGlobal(float &x, float &y) const = 0;
	/**
	* Выполняет преобразование точки в глобальной системе координат в локальную
	*/
	virtual void globalToLocal(float &x, float &y) const = 0;
	/**
	* Выполняет преобразование локальных координат в координаты целевого объекта
	*/
	virtual void localToTarget(float &x, float& y, IFlashDisplayObject* target) const = 0;

	/**
	* Функция, обновляющая состояние объекта. В потомках ее следует переопределять вместе с hasUpdate
	*/
	virtual void update(float dt) = 0;
	/**
	* Имеется ли функция update. Должна возвращать константу.
	*/
	virtual bool hasUpdate() const = 0;

	/**
	* Устанавливает родительский объект
	* 'Safe conditions': вызывается из метода объекта parent, который уже
	* поместил текущий объект в свой список потомков.
	*/
	virtual void unsafeSetParent(IFlashSprite* parent) = 0;

	/**
	* Получение родительского объекта
	*/
	virtual IFlashSprite* getParent() const = 0;
	/**
	* Эффективная реализация получения общего предка
	* O(n + m), где n, m - высота деревьев this и another.
	*/
	virtual IFlashDisplayObject* getCommonAncestor(IFlashDisplayObject* another) const = 0;
	/**
	* Получение корневого объекта - самого дальнего предка, не имеющего родителя
	*/
	virtual IFlashDisplayObject* getRoot() = 0;

	/**
	* Устанавливает скорость обновления DisplayObject в долях от нормальной
	*/
	virtual void setUpdateRate(float rate) = 0;
	virtual float getUpdateRate() const = 0;

	/**
	* Устанавливает пользовательскую операцию рисования
	*/
	virtual void setCustomDrawOperation(IFlashCustomDrawOperation*) = 0;

	/**
	*	Для обработки скролла колесиком мышки
	*/
	virtual void mouseWheel(int delta) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
