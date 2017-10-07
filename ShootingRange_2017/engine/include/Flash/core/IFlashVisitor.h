#pragma once

struct IFlashDisplayObject;

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

/**
* Коллбек для функции обхода flash-иерархии 
* @see IFlashDisplayObject::visitBFS
* @see IFlashDisplayObject::visitDFS
*/
struct IFlashVisitor
{
	virtual bool visit(IFlashDisplayObject* displayObject) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
