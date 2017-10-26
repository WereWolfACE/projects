#pragma once

#include "IPrototype.h"
#include "IFlashSprite.h"
#include "FlashDisplayObjectContainer.hpp"
#include "FlashUnknown.hpp"

/**
* FlashDisplayObject, который является контейнером для других объектов.
*/
class FlashSprite: 
	public IPrototype,
	public FlashDisplayObjectContainer<IFlashSprite>
{
BEGIN_INTERFACE_MAP()
	INTERFACE_ENTRY(IPrototype)
	INTERFACE_ENTRY(IFlashSprite)
	INTERFACE_ENTRY(IFlashDisplayObject)
END_INTERFACE_MAP()

public:
	FlashSprite();
	FlashSprite(FlashSprite& _ref);
	virtual ~FlashSprite();
};
