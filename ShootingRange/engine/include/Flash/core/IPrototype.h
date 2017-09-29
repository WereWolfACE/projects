#pragma once

#include "IFlashUnknown.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

struct IPrototype: public IFlashUnknown
{
	virtual IPrototype* Clone() = 0;

	virtual IPrototype* getRootPrototype() = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
