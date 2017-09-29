#ifndef __ADDANIMATIONARGS_H__
#define __ADDANIMATIONARGS_H__

#pragma once
#include "AnimationType.h"

///
/// Аргументы события OnAddAnimation
///
class AddAnimationArgs : public EventArgs 
{
public:
	AddAnimationArgs(std::string name, FPoint position, AnimationType type)
	{
		_type = type;
		_name = name;
		_pos = position;
	}	
	std::string GetName()
	{
		return _name;
	}
	FPoint GetPosition()
	{
		return _pos;
	}
	AnimationType GetType()
	{
		return _type;
	}
	
private:
	AnimationType _type;
	std::string _name;
	FPoint _pos;
};

#endif //__ADDANIMATIONARGS_H__