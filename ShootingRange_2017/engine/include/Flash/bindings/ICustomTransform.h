#pragma once

#include "RefCounter.h"

class ICustomTransform: public RefCounter {
public:
	virtual void apply(float,float*,float*,float*,float*,unsigned char*) = 0;
};

ICustomTransform* tform_function(void(*)(float,float*,float*));
ICustomTransform* tform_function(void(*)(float,float*,float*,float*,float*,unsigned char*));
