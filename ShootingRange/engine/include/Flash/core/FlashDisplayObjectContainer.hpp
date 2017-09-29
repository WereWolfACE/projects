#pragma once

#include <algorithm>
#include <vector>
#include <boost/bind.hpp>

#include "FlashDisplayObject.hpp"
#include "FlashUtility.hpp"
#include "IHitTestDelegate.h"
#include "IFlashDisplayObject.h"
#include "IFlashVisitor.h"
#include "IPrototype.h"
#include "IFlashSprite.h"

#include "Render/FlashRender.h"

#include "../events/FlashEvents.h"
#include "../events/EventManager.h"

/*
	Базовый клас реализации интерфейса IFlashSprite

	Реализует базовую функциональность котейнера флешовых объектов, к примеру от него унаследован FlashMovieClip и FlashSprite

	Всех наследников этого класса следует создавать по следующему сценарию:

	IFlashDisplayObject* l_obj = new CPrototype<FlashDisplayObjectContainer>();
	В этом случае объект бдует иметь возможность клонироваться (метод Clone интрефейса IPrototype),
	но для правильной работы клонирование необхождимо описать корректный конструктор копирования

	или

	IFlashDisplayObject* l_obj = new CFlashUnknown<FlashDisplayObjectContainer>();
	Если не требуется функциональность клонирования

	С полной диаграммой классов можно ознакомится по ссылке ниже:
	https://drive.google.com/a/playrix.com/file/d/0Bziu0DiTP-9vSjNteDFDSDFSQkU/view?usp=sharing
*/
template<class T>
class FlashDisplayObjectContainer: public FlashDisplayObject<T>
{
public:
	FlashDisplayObjectContainer():
		numUpdateListeners(0)
	{
	};

	FlashDisplayObjectContainer(FlashDisplayObjectContainer& _ref):
		FlashDisplayObject<T>(_ref),
		__m_children(_ref.__m_children.size()),
		numUpdateListeners(_ref.numUpdateListeners)
	{
		auto l_childIt = __m_children.begin();

		for(auto l_it = _ref.__m_children.begin(); l_it != _ref.__m_children.end(); ++l_it)
		{
			IFlashDisplayObject* l_dispObj = *l_it;
			IFlashDisplayObject* l_newDispObj = NULL;

			if(l_dispObj != NULL)
			{
				IPrototype* l_dispObjProto = l_dispObj->Prototype();
				l_newDispObj = l_dispObjProto->Clone()->DisplayObject();
				l_newDispObj->unsafeSetParent(this);
			};

			*l_childIt++ = l_newDispObj;
		};
	};

	//-------------------------------------------------------------------------
	//
	//                          IFlashDisplayObject
	//
	//-------------------------------------------------------------------------
	void unsafeSetParent(IFlashSprite* _parent) override
	{
		if (this->parent != _parent)
		{
			if (this->parent != nullptr)
			{
				this->parent->changeUpdateListeners(-numUpdateListeners);
			};

			if (_parent)
			{
				_parent->changeUpdateListeners(numUpdateListeners);
			};
		};

		FlashDisplayObject<T>::unsafeSetParent(_parent);
	};

	void mouseWheel(int delta) override
	{
		for(auto l_it = __m_children.begin(); l_it != __m_children.end(); ++l_it)
		{
			IFlashDisplayObject* l_child = *l_it;
			if ( !(!l_child->getVisible() || l_child->getAlpha() <= 0.0f))
			{
				l_child->mouseWheel(delta);
			}
		}
	};

	void render(FlashRender& _render) override
	{
		if ( this->operation ){
			if ( !this->operation->onPreRender(this, _render) ){
				return;
			}
		}

		for(auto l_it = __m_children.begin(); l_it != __m_children.end(); ++l_it)
		{
			IFlashDisplayObject* l_child = *l_it;

			if ( !(!l_child->getVisible() || l_child->getAlpha() <= 0.0f))
			{
				float childMatrix[6];
				l_child->getMatrix(childMatrix);
				const Color& childColor = l_child->getColor();

				bool identityTransform = ::matrixIdentity(childMatrix);
				bool identityColor = childColor == Color::WHITE;

				float localMatrix[6];
				Color localColor;

				if ( !identityTransform ){
					_render.saveMultMatrix(childMatrix, localMatrix);
				}
				if ( !identityColor ){
					localColor = _render.setColor(_render.getColor() * childColor);
				}

				l_child->render(_render);

				if ( !identityTransform ){
					_render.restoreMatrix(localMatrix);
				}
				if ( !identityColor ){
					_render.setColor(localColor);
				}
			};
		}

		if ( this->operation )
		{
			this->operation->onPostRender(this, _render);
		}
	};

	bool hitTest(float x, float y, IHitTestDelegate* hitTestDelegate) override
	{
		if ( this->mouseTransparent )
			return false;

		for (auto l_it = __m_children.rbegin(); l_it != __m_children.rend(); ++l_it)
		{
			IFlashDisplayObject* l_child = *l_it;

			if (l_child->getVisible() && !l_child->getHitTestTransparent())
			{
				float localX = x;
				float localY = y;
				l_child->parentToLocal(localX, localY);

				if ( l_child->hitTest(localX, localY, hitTestDelegate) )
				{
					if ( !this->skipMouseEvents )
					{
						GC_STACKREF(this)
						hitTestDelegate->receiveNext(this, x, y);
					}

					return true;
				}
			};
		}

		return false;
	};

	bool getBounds(float& left, float& top, float& right, float& bottom, IFlashDisplayObject* targetCoordinateSystem) override
	{
		if ( !targetCoordinateSystem )
			targetCoordinateSystem = this;

		if (__m_children.empty())
		{
			left = top = 0;
			this->localToTarget(left, top, targetCoordinateSystem);
			right = left;
			bottom = top;

			return false;
		}
		else
		{
			bool initBounds = false;

			for (auto l_it = __m_children.begin(); l_it != __m_children.end(); ++l_it)
			{
				IFlashDisplayObject* l_child = *l_it;

				if ( initBounds ){
					float cleft, ctop, cright, cbottom;
					if ( l_child->getBounds(cleft, ctop, cright, cbottom, targetCoordinateSystem) ){
						expand(left, right, cleft, cright);
						expand(top, bottom, ctop, cbottom);
					}
				}else{
					if ( l_child->getBounds(left, top, right, bottom, targetCoordinateSystem) ){
						initBounds = true;
					}
				}
			}

			return initBounds;
		}
	};

	void advance(FlashUpdateListener* updateListener, float dt) override
	{
		GC_STACKREF(static_cast<IFlashSprite*>(this))
		FlashDisplayObject<T>::advance(updateListener, dt);

		if ( this->numUpdateListeners > 0 && !__m_children.empty())
		{
			for (auto l_child : __m_children)
			{
				float rate = l_child->getUpdateRate();

				if ( rate != 0.0f )
				{
					l_child->advance(updateListener, dt * rate);
				};
			};
		};
	};

	//-------------------------------------------------------------------------
	//
	//                              IFlashSprite
	//
	//-------------------------------------------------------------------------
	void changeUpdateListeners(int _value) override
	{
		if (_value)
		{
			numUpdateListeners += _value;
			//			Assert(numUpdateListeners>=0);

			if (this->parent)
			{
				this->parent->changeUpdateListeners(_value);
			};
		};
	}

	IFlashDisplayObject* getChildByName(const std::string& _name) override
	{
		auto l_findIt = std::find_if(__m_children.begin(), __m_children.end(), [_name](IFlashDisplayObject* _a){return _a->getName() == _name;});
		return (l_findIt != __m_children.end())?*l_findIt:NULL;
	};

	IFlashDisplayObject* addChild(IFlashDisplayObject* _displayObject) override
	{
		auto l_removeIt = std::remove(__m_children.begin(), __m_children.end(), _displayObject);

		if(l_removeIt == __m_children.end())
		{
			IFlashSprite* l_parentStrite = _displayObject->getParent();

			//если у объекта уже есть родитель то отсоединяем детенка от него
			if (l_parentStrite != nullptr)
			{
				l_parentStrite->removeChild(_displayObject);
			};

			_displayObject->unsafeSetParent(this);
		}
		else
		{
			__m_children.erase(l_removeIt, __m_children.end());
		};

		__m_children.push_back(_displayObject);

		return _displayObject;
	};

	IFlashDisplayObject* removeChild(IFlashDisplayObject* _displayObject) override
	{
		int l_childIndex = __getChildIndex(_displayObject);

		return (l_childIndex >= 0)?removeChildAt(l_childIndex):NULL;
	};

	int getChildIndex(IFlashDisplayObject* _displayObject) const override
	{
		return __getChildIndex(_displayObject);
	};

	IFlashDisplayObject* getChildAt(int _index) override
	{
		if ( _index < 0 || _index >= (int)__m_children.size() )
			return NULL;

		return __m_children[_index];
	}

	int getChildrenCount() const override
	{
		return (int)__m_children.size();
	}

	IFlashDisplayObject* addChildAt(IFlashDisplayObject* _displayObject, int _index) override
	{
//		Assert(_index >= 0 && _index <= (int)__m_children.size());
//		Assert(_displayObject != 0);
//		Assert(displayObject->GetMark() != 0xff);

		IFlashSprite* l_parentStrite = _displayObject->getParent();

		if (l_parentStrite && l_parentStrite != this)
		{
			l_parentStrite->removeChild(_displayObject); //пусть уж лучше упадет чем просто промолчит
		};

		auto l_insertedElmIt = std::find(__m_children.begin(), __m_children.end(), _displayObject);

		if(l_insertedElmIt != __m_children.end())
		{
			if(l_insertedElmIt < (__m_children.begin() + _index))
			{
				std::rotate(l_insertedElmIt, l_insertedElmIt + 1, __m_children.begin() + _index);
			}
			else
			{
				std::rotate(__m_children.begin() + _index, l_insertedElmIt, l_insertedElmIt + 1);
			};
		}
		else
		{
			__m_children.insert(__m_children.begin() + _index, _displayObject);
		};

		_displayObject->unsafeSetParent(this);

		FlashEvent event(Added, false);
		EventManager::Get().dispatchTarget(_displayObject, event);

		return _displayObject;
	};

	IFlashDisplayObject* removeChildAt(int _index) override
	{
//		Assert(_index >= 0 && index < (int)__m_children.size());
		IFlashDisplayObject* l_child = __m_children[_index];
		l_child->unsafeSetParent(NULL);

		__m_children.erase(__m_children.begin() + _index);

		FlashEvent event(Removed, false);
		EventManager::Get().dispatchTarget(l_child, event);

		return l_child;
	};

	void swapChildren(int _index1, int _index2) override
	{
		Assert2(0 <= _index1 && _index1 < (int)__m_children.size(), "invalid index for swap");
		Assert2(0 <= _index2 && _index2 < (int)__m_children.size(), "invalid index for swap");

		IFlashDisplayObject* l_temp = __m_children[_index2];
		__m_children[_index2] = __m_children[_index1];
		__m_children[_index1] = l_temp;
	};

	void visitDFS(IFlashVisitor* visitor, bool preorder) override
	{
		for(auto l_it = __m_children.begin(); l_it != __m_children.end(); ++l_it)
		{
			IFlashDisplayObject* l_child = *l_it;

			if ( preorder )
			{
				visitor->visit(l_child);
			};

			IFlashSprite* l_subtree = l_child->Sprite();

			if (l_subtree)
			{
				l_subtree->visitDFS(visitor, preorder);
			};

			if ( !preorder )
			{
				visitor->visit(l_child);
			};
		};
	};

	void visitBFS(IFlashVisitor* visitor, bool preorder) override
	{
		if ( preorder )
		{
			for (auto l_it = __m_children.begin(); l_it != __m_children.end(); ++l_it)
			{
				visitor->visit(*l_it);
			}
		}

		for (auto l_it = __m_children.begin(); l_it != __m_children.end(); ++l_it)
		{
			IFlashDisplayObject* l_child = *l_it;
			IFlashSprite* l_subtree = l_child->Sprite();

			if (l_subtree)
			{
				l_subtree->visitBFS(visitor, preorder);
			};
		}

		if ( !preorder )
		{
			for (auto l_it = __m_children.begin(); l_it != __m_children.end(); ++l_it)
			{
				visitor->visit(*l_it);
			};
		};
	};

	//-------------------------------------------------------------------------
	//
	//
	//
	//-------------------------------------------------------------------------
private:
	int __getChildIndex(IFlashDisplayObject* _displayObject) const
	{
		// http://stackoverflow.com/questions/2152986/best-way-to-get-the-index-of-an-iterator
		auto l_childBegin = __m_children.begin();
		auto l_childEnd = __m_children.end();
		auto l_findIt = std::find(l_childBegin, l_childEnd, _displayObject);

		return (l_findIt != l_childEnd)?int(l_findIt - l_childBegin):-1;
	};

protected:
	GC_BLACKEN_DECL()
	{
		GC_BLACKEN_CALL(FlashDisplayObject<T>);

		for (auto l_childIt = __m_children.begin(); l_childIt != __m_children.end(); ++l_childIt)
		{
			if ( *l_childIt != NULL)
			{
				GC_SHOULD_MARK(*l_childIt);
			};
		};
	};

protected:
	std::vector<IFlashDisplayObject*> __m_children;
	int numUpdateListeners;
};
