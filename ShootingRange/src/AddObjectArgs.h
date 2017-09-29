#ifndef __ADDOBJECTARGS_H__
#define __ADDOBJECTSARGS_H__

#pragma once
#include "BaseObject.h"

///
/// Аргументы события OnAddObjects
///
class AddObjectArgs : public EventArgs 
{
public:
	AddObjectArgs(std::weak_ptr<BaseObject> object)
	{
		_object = object;
	}
	std::weak_ptr<BaseObject> GetChangedObject()
	{
		return _object;
	}
private:
	std::weak_ptr<BaseObject> _object;
};

#endif //__ADDOBJECTARGS_H__