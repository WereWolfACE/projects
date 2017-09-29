#pragma once

#include "IGCRef.h"

/*
	Класс реализующий функциональность интрефейса IGCRef,
	Класс реализует подсчет ссылок(для корневых объектов), и пометку(SetMark) при сборках мусора

	так как все интерфейсы во флеше в конечном итоге имеют общего предка в виде IGCRef,
	то возникает проблема виртуального наследования(http://habrahabr.ru/post/185826/)
	CGCRef пытается решить данную пролему за счет следующего трюка:

	Допустим у нас есть класс FlashDisplayObject,
	который реализует два интрерфейса IFlashDisplayObject и IPrototype,
	они оба будут имет общую частть в ввиде IGCRef,
	и если мы зададим следующее выражение при создании экземпляра класса:

	l_obj = new CGCRef<FlashDisplayObject>();

	это приведет к тому что будет созда новый класс CGCRef<FlashDisplayObject>,
	который унаследован от FlashDisplayObject, и реализовывает функциональность IGCRef, но как бы в конце наслодования,
	тем самым решая проблему виртуального наследования, но это накладывает и определнные ограничения на класс FlashDisplayObject,
	у него не может быть параметризированного конструктора

	В принципе в 99% случаев можно не задумываться об подобной функциональности, потому как она уже есть,
	и состредоточится на написании непосредственно функциональной составляющей физуальных компонент флеша

	С полной диаграммой классов можно ознакомится по ссылке ниже:
	https://drive.google.com/a/playrix.com/file/d/0Bziu0DiTP-9vSjNteDFDSDFSQkU/view?usp=sharing

*/
template<class T>
class CGCRef: public T
{
public:
	CGCRef():
		__m_refcount(0)
	{
	};

	CGCRef(CGCRef& ref):
		T(ref),
		__m_refcount(0)
	{
	};

	virtual ~CGCRef()
	{
	};

	int AddRef() override
	{
		return ++__m_refcount;
	};

	int GetRef() const override
	{
		return __m_refcount;
	};

	int Release() override
	{
		int l_recount = --__m_refcount;

		if(l_recount == 0)
		{
			delete this;
		}

		return l_recount;
	};

	void SetMark(int mark) override
	{
		__m_mark = mark;
	};

	int GetMark() const override
	{
		return __m_mark;
	};

	void gcBlacken()
	{
		T::__gcBlacken();
	};

private:
	int __m_refcount;
	short __m_mark;
};

void intrusive_ptr_add_ref(IGCRef* p);
void intrusive_ptr_release(IGCRef* p);
