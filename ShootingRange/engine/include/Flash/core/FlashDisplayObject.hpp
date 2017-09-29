#pragma once

#include <Utils/Color.h>

#include "IFlashDisplayObject.h"
#include "IFlashSprite.h"
#include "IFlashCustomDrawOperation.h"
#include "FlashMarkSweep.h"
#include "FlashUtility.hpp"
#include "FlashUpdateListener.hpp"
#include "Flash/core/IPrototype.h"
#include "Flash/core/IFlashMovieClip.h"

/*
	Базовый клас реализации интерфейса IFlashDisplayObject(все кастомные объекты следует наследовать от него)
	в таком виде:

	class FlashDisplayObjectChild: public FlashDisplayObject<IFlashDisplayObject>
	{
		...
	}

	Всех наследников этого класса следует создавать по следующему сценарию:

	IFlashDisplayObject* l_obj = new CPrototype<FlashDisplayObjectChild>();
	В этом случае объект бдует иметь возможность клонироваться (метод Clone интрефейса IPrototype),
	но для правильной работы клонирование необхождимо описать корректный конструктор копирования

	или

	IFlashDisplayObject* l_obj = new CFlashUnknown<FlashDisplayObjectChild>();
	Если не требуется функциональность клонирования

	С полной диаграммой классов можно ознакомится по ссылке ниже:
	https://drive.google.com/a/playrix.com/file/d/0Bziu0DiTP-9vSjNteDFDSDFSQkU/view?usp=sharing

*/
template<class T>
class FlashDisplayObject: public T
{
public:
	FlashDisplayObject():
		parent(NULL),
		updateRate(1),
		decomposition(),
		name(""),
		operation(NULL),
		mouseTransparent(false),
		skipMouseEvents(false),
		ownVisible(true),
		autoPlay(true),
		decompositionValid(false),
		ownColor(Color::WHITE)
	{
		::memcpy(ownMatrix, identityMatrix, 6 * sizeof(float));
	};

	/*
		При кажущейся излишности конструктор копированя тут необходим, потому как при создании объектов через шаблон (CPrototype<FlashDisplayObjectContainer>),
		конструктор копирования базового класса(CPrototype) не вызовется если тут не переопределить конструктор копирования
	*/
	FlashDisplayObject(FlashDisplayObject& _ref):
		parent(NULL),
		updateRate(_ref.updateRate),
		decomposition(_ref.decomposition),
		name(_ref.name),
		operation(_ref.operation),
		mouseTransparent(_ref.mouseTransparent),
		skipMouseEvents(_ref.skipMouseEvents),
		ownVisible(_ref.ownVisible),
		autoPlay(_ref.autoPlay),
		decompositionValid(_ref.decompositionValid),
		ownColor(_ref.ownColor)
	{
		::memcpy(ownMatrix, _ref.ownMatrix, sizeof(ownMatrix));
	};

	virtual ~FlashDisplayObject()
	{
	};

	void applyTransform(IFlashDisplayObject* another) override
	{
		float A[9];
		float B[9];
		float C[9];
		this->getMatrix(A);
		another->getMatrix(B);
		::multMatrix3x3(C, A, B);
		this->setMatrix(C);

		ownColor *= another->getColor();

		this->setVisible(another->getVisible() && this->getVisible());
	};

	bool resetTransform() override
	{
		IPrototype* prototype = this->Prototype();
		if (prototype && prototype->getRootPrototype()) {
			prototype = prototype->getRootPrototype();
			if (prototype) {
				if (prototype->MovieClip()) {
					prototype->MovieClip()->setMatrix(identityMatrix);
					prototype->MovieClip()->gotoFrame(0);
					return true;
				}
				else if (prototype->DisplayObject()) {
					float m[9];
					prototype->DisplayObject()->getMatrix(m);
					setMatrix(m);
					return true;
				}
			}
		}
		return false;
	}

	void advance(FlashUpdateListener* updateListener, float dt) override
	{
		updateListener->addUpdateRequest(this, dt);
	};

	void setHitTestTransparent(bool _value) override
	{
		mouseTransparent = _value;
	};

	bool getHitTestTransparent() const override
	{
		return mouseTransparent;
	};

	void setHitTestDispatcher(bool _value) override
	{
		skipMouseEvents = !_value;
	};

	bool getHitTestDispatcher() const override
	{
		return !skipMouseEvents;
	};

	void setVisible(bool value) override
	{
		ownVisible = value;
	};

	bool getVisible() const override
	{
		return ownVisible;
	};

	void setName(const std::string& _name) override
	{
		name = _name;
	};

	const std::string& getName() const override
	{
		return name;
	};

	void setAlpha(float value) override
	{
		ownColor.alpha = value * 0xFF;
	};

	float getAlpha() const override
	{
		return ownColor.alpha / (float)0xFF;
	};

	void setColor(const Color& color) override
	{
		ownColor = color;
	};

	const Color& getColor() const override
	{
		return ownColor;
	};

	void setPosition(float x, float y) override
	{
		ownMatrix[2] = x;
		ownMatrix[5] = y;
	};

	void getPosition(float& x, float& y) override
	{
		if ( !decompositionValid )
			__decompose();

		x = ownMatrix[2];
		y = ownMatrix[5];
	};

	void setRotation(float radians) override
	{
		if ( !decompositionValid )
			__decompose();

		decomposition.rotation = radians;
		recompose(ownMatrix, decomposition);
	};

	float getRotation() override
	{
		if ( !decompositionValid )
			__decompose();

		return decomposition.rotation;
	};

	void setShear(float k) override
	{
		if ( !decompositionValid )
			__decompose();

		decomposition.shear = k;
		recompose(ownMatrix, decomposition);
	};

	float getShear() override
	{
		if ( !decompositionValid )
			__decompose();

		return decomposition.shear;
	};

	void setScale(float scaleX, float scaleY) override
	{
		if ( !decompositionValid )
			__decompose();

		decomposition.scaleX = scaleX;
		decomposition.scaleY = scaleY;
		recompose(ownMatrix, decomposition);
	};

	void getScale(float& x, float& y) override
	{
		if ( !decompositionValid )
			__decompose();

		x = decomposition.scaleX;
		y = decomposition.scaleY;
	};

	void setMatrix(const float *matrix) override
	{
		::memcpy((uint8_t*)ownMatrix, (uint8_t*)matrix, sizeof(ownMatrix));
		decompositionValid = false;
	};

	void getMatrix(float *matrix) const override
	{
		::memcpy((uint8_t*)matrix, (uint8_t*)ownMatrix, sizeof(ownMatrix));
	};

	void localToParent(float& x, float& y) const override
	{
		float x0 = x * ownMatrix[0] + y * ownMatrix[1] + ownMatrix[2];
		float y0 = x * ownMatrix[3] + y * ownMatrix[4] + ownMatrix[5];

		x = x0;
		y = y0;
	};

	void parentToLocal(float& x, float& y) const override
	{
		float det = 1.0f / (ownMatrix[0] * ownMatrix[4] - ownMatrix[1] * ownMatrix[3]);
		float tx = det * (ownMatrix[1] * ownMatrix[5] - ownMatrix[4] * ownMatrix[2]);
		float ty = det * (ownMatrix[3] * ownMatrix[2] - ownMatrix[0] * ownMatrix[5]);
		float a = det * ownMatrix[4];
		float b = -det * ownMatrix[3];
		float c = -det * ownMatrix[1];
		float d = det * ownMatrix[0];

		float x0 = x * a + y * c + tx;
		float y0 = x * b + y * d + ty;

		x = x0;
		y = y0;
	};

	void localToGlobal(float &x, float &y) const override
	{
		const IFlashDisplayObject* displayObject = this;

		while ( displayObject )
		{
			displayObject->localToParent(x, y);
			displayObject = displayObject->getParent();
		};
	};

	void globalToLocal(float &x, float &y) const override
	{
		if ( parent )
			parent->globalToLocal(x, y);

		parentToLocal(x, y);
	};

	void localToTarget(float &x, float& y, IFlashDisplayObject* target) const override
	{
		if ( target == 0 ){
			//по определению
			localToGlobal(x, y);

		//Несколько частных случаев для ускорения работы (getCommonAncestor дорогой)
		}else if ( target == this ){
			return;
		}else if ( target == parent ){
			return localToParent(x, y);
		}else if ( target->getParent() == parent ){
			localToParent(x, y);
			target->parentToLocal(x, y);
		}else{
			const IFlashDisplayObject* commonAncestor = getCommonAncestor(target);

			if ( !commonAncestor ){
				localToGlobal(x, y);
				target->globalToLocal(x, y);
			}else{
				const IFlashDisplayObject* current = this;
				while ( current != commonAncestor ){
					current->localToParent(x, y);
					current = current->getParent();
				}
				__ancestorToLocal(x, y, current, target);
			}
		}
	};

	void update(float dt) override
	{
	};

	bool hasUpdate() const override
	{
		return false;
	};

	void unsafeSetParent(IFlashSprite* _parent) override
	{
		if(parent != _parent)
		{
			int l_incval = hasUpdate() ? 1 : 0;

			if (parent != NULL)
			{
				parent->changeUpdateListeners(-l_incval);
			};

			parent = _parent;

			if (parent )
			{
				parent->changeUpdateListeners(l_incval);
			};
		};
	};

	IFlashSprite* getParent() const override
	{
		return parent;
	};

	IFlashDisplayObject* getCommonAncestor(IFlashDisplayObject* another) const override
	{
		IFlashDisplayObject* l_retval = nullptr;

		if (another)
		{
			if (this == another)
			{
				return another;
			}

			IFlashDisplayObject* lTree[256];
			IFlashDisplayObject* rTree[256];
			int lPtr = 0, rPtr = 0;

			for (IFlashDisplayObject* l_left = this->getParent(); l_left; l_left = l_left->getParent())
			{
				lTree[lPtr] = l_left;
				lPtr += 1;
			};

			for (IFlashDisplayObject* l_right = another; l_right; l_right = l_right->getParent())
			{
				rTree[rPtr] = l_right;
				rPtr += 1;
			};

			lPtr--;
			rPtr--;

			while (lPtr >= 0 && rPtr >= 0 && (lTree[lPtr] == rTree[rPtr]))
			{
				l_retval = lTree[lPtr];

				lPtr--;
				rPtr--;
			};

			if (rPtr >= 0 && (this == rTree[rPtr]))
			{
				l_retval = rTree[rPtr];
			};
		};

		return l_retval;
	}

	IFlashDisplayObject* getRoot() override
	{
		IFlashDisplayObject* l_retval = this;

		while (l_retval->getParent())
			l_retval = l_retval->getParent();

		return l_retval;
	};

	void setUpdateRate(float rate) override
	{
		updateRate = rate;
	};

	float getUpdateRate() const override
	{
		return updateRate;
	};

	void setCustomDrawOperation(IFlashCustomDrawOperation* _operation) override
	{
		operation = _operation;
	};

	void mouseWheel(int delta) override {
	};
private:
	void __decompose()
	{
		decomposition = decompose(ownMatrix);
		decompositionValid = true;
	};

	void __ancestorToLocal(float &x, float& y, const IFlashDisplayObject* ancestor, const IFlashDisplayObject* target) const
	{
		if ( ancestor == target ){
			return;
		}else if ( ancestor == target->getParent() ){
			target->parentToLocal(x, y);
		}else{
			__ancestorToLocal(x, y, ancestor, target->getParent());
			target->parentToLocal(x, y);
		}
	};
protected:
	GC_BLACKEN_DECL()
	{
		if ( parent )
			GC_SHOULD_MARK(parent);

		if ( operation )
			GC_SHOULD_MARK(static_cast<IGCRef*>(operation));
	};

public:
	IFlashSprite* parent;
	IFlashCustomDrawOperation* operation;

	float ownMatrix[6];
	Color ownColor;

	float updateRate;

	MatrixDecomposition decomposition;

	std::string name;

	bool mouseTransparent:1;
	bool skipMouseEvents:1;
	bool ownVisible:1;
	bool autoPlay:1;
	bool decompositionValid:1;
};
