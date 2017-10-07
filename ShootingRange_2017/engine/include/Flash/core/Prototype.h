#pragma once

#include <boost/intrusive_ptr.hpp>

#include "IPrototype.h"
#include "IFlashDisplayObject.h"
#include "FlashUnknown.hpp"
#include "FlashMarkSweep.h"

/*
	CPrototype реализация объектов которые желают реализовывать функциональность клонирования, за деталями можно обратиться к коментариям в GCRef.h
*/
template<class T>
class CPrototype: public CFlashUnknown<T>
{
public:
	virtual ~CPrototype()
	{
	};

	IPrototype* Clone()
	{
		CPrototype<T>* l_obj = new CPrototype<T>(*this);
		GC_SHOULD_TRACK(static_cast<IPrototype*>(l_obj)); //это нельзя поместить в конструктор, так как в этот момент вызывается виртуальный метод, а таблица виртуальных методов как известно в момент констрирования ещё не проинициализирована

		return l_obj;
	};

	IPrototype* getRootPrototype()
	{
		return __m_root.get();
	}

	/*
		Содержит ссылку на корневой объект CPrototypeWeak, а тот в свою очередь удерживает ссылку на swl библиотеку,
		не давая ей выгружаться покак из неё используется хоть один элемент

		Если CPrototype используется для кастомных классов то на этот элемент можно не обращать внимания он всегда в этом случае будет содержать nullptr
	*/
	boost::intrusive_ptr<IPrototype> __m_root;
};
