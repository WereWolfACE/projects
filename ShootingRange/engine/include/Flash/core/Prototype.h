#pragma once

#include <boost/intrusive_ptr.hpp>

#include "IPrototype.h"
#include "IFlashDisplayObject.h"
#include "FlashUnknown.hpp"
#include "FlashMarkSweep.h"

/*
	CPrototype ���������� �������� ������� ������ ������������� ���������������� ������������, �� �������� ����� ���������� � ����������� � GCRef.h
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
		GC_SHOULD_TRACK(static_cast<IPrototype*>(l_obj)); //��� ������ ��������� � �����������, ��� ��� � ���� ������ ���������� ����������� �����, � ������� ����������� ������� ��� �������� � ������ �������������� ��� �� �������������������

		return l_obj;
	};

	IPrototype* getRootPrototype()
	{
		return __m_root.get();
	}

	/*
		�������� ������ �� �������� ������ CPrototypeWeak, � ��� � ���� ������� ���������� ������ �� swl ����������,
		�� ����� �� ����������� ����� �� �� ������������ ���� ���� �������

		���� CPrototype ������������ ��� ��������� ������� �� �� ���� ������� ����� �� �������� �������� �� ������ � ���� ������ ����� ��������� nullptr
	*/
	boost::intrusive_ptr<IPrototype> __m_root;
};
