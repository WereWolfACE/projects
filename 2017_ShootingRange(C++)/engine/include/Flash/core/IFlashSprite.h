#pragma once

#include "IFlashDisplayObject.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

//TODO переименовать в IFlashDisplayObjectContainer
struct IFlashSprite : public IFlashDisplayObject
{
	/**
	* »змен€ет число потомков в иерархии, желающих получать событие update.
	*/
	virtual void changeUpdateListeners(int value) = 0;

	virtual IFlashDisplayObject* getChildByName(const std::string& name) = 0;
	virtual IFlashDisplayObject* addChild(IFlashDisplayObject* displayObject) = 0;
	virtual IFlashDisplayObject* removeChild(IFlashDisplayObject* displayObject) = 0;
	virtual IFlashDisplayObject* addChildAt(IFlashDisplayObject* displayObject, int index) = 0;
	virtual IFlashDisplayObject* removeChildAt(int index) = 0;
	virtual IFlashDisplayObject* getChildAt(int index) = 0;
	virtual int getChildrenCount() const = 0;
	virtual int getChildIndex(IFlashDisplayObject* displayObject) const = 0;
	virtual void swapChildren(int _index1, int _index2) = 0;

		/**
	* –екурсивных обход иерархии в глубину.
	* @param preorder определ€ет, будет ли текущ€€ вершина посещена до (true) либо после (false) дочерних
	*/
	virtual void visitDFS(IFlashVisitor* visitor, bool preorder) = 0;
	/**
	* –екурсивных обход иерархии в ширину.
	* @param preorder определ€ет, будет ли текущ€€ вершина посещена до (true) либо после (false) дочерних
	*/
	virtual void visitBFS(IFlashVisitor* visitor, bool preorder) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
