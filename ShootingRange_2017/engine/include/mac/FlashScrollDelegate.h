//
//  FlashScrollDelegate.h
//  Engine_Mac
//
//  Created by True on 04/05/15.
//
//

#ifndef __Engine_Mac__FlashScrollDelegate__
#define __Engine_Mac__FlashScrollDelegate__

#include "RefCounter.h"
#include "Flash/core/IPrototype.h"
#include "Flash/core/FlashUnknown.hpp"
#include "Flash/bindings/ScrollingContainer.h"

class IFlashDisplayObject;
class FlashScrollDelegate: public RefCounter
{
public:
    virtual void scrollCreated(CFlashUnknown<ScrollingContainer>* scrollingContainer) = 0;
    
    static void invokeScrollCreated(CFlashUnknown<ScrollingContainer>* scrollingContainer);
    static void resetDelegate(const boost::intrusive_ptr<FlashScrollDelegate>& delegate = boost::intrusive_ptr<FlashScrollDelegate>());
    
private:
    struct Impl;
};


#endif /* defined(__Engine_Mac__FlashScrollDelegate__) */
