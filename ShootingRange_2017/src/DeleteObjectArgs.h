#ifndef __DELETEOBJECTARGS_H__
#define __DELETEOBJECTARGS_H__

#pragma once
#include "BaseObject.h"

///
/// Аргументы события OnDeleteObjectEvent
///
class BaseObject;

class DeleteObjectArgs : public EventArgs 
{
public:	
	DeleteObjectArgs(BaseObject* object)
	{
		_object = object;		
	}
	BaseObject* GetChangedObject()
	{
		return _object;
	}	
private:
	BaseObject* _object;	
};

#endif //__DELETEOBJECTARGS_H__