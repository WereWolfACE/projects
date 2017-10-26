//
//  FlashButtonDelegate.h
//  Engine_Mac
//
//  Created by True on 04/05/15.
//
//

#ifndef __Engine_Mac__FlashButtonDelegate__
#define __Engine_Mac__FlashButtonDelegate__

#include "RefCounter.h"

struct IFlashDisplayObject;
class FlashButtonDelegate: public RefCounter
{
public:
    virtual void buttonCreated(IFlashDisplayObject* displayObject) = 0;

    static void invokeButtonCreated(IFlashDisplayObject* displayObject);
    static void resetDelegate(const boost::intrusive_ptr<FlashButtonDelegate>& delegate = boost::intrusive_ptr<FlashButtonDelegate>());

private:
    struct Impl;
};

#endif /* defined(__Engine_Mac__FlashButtonDelegate__) */
