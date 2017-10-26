#ifndef __BORDER_H__
#define __BORDER_H__

#pragma once
#include "BaseObject.h"

class Border : public BaseObject
{
public:
	Border(FPoint begin, FPoint end);
	FPoint GetBegin() const;
	FPoint GetEnd() const;	
	void Init() override;
	void Draw() override;
	~Border();

private:		
	FPoint _positionEnd;
};

#endif //__BORDER_H__
