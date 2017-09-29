#ifndef __Engine_Mac__FlashPressableDelegate__
#define __Engine_Mac__FlashPressableDelegate__

#include "RefCounter.h"

struct IFlashDisplayObject;
class FlashPressableDelegate: public RefCounter
{
public:
    virtual void pressableCreated(IFlashDisplayObject* displayObject) = 0;

    static void invokePressableCreated(IFlashDisplayObject* displayObject);
    static void resetDelegate(const boost::intrusive_ptr<FlashPressableDelegate>& delegate = boost::intrusive_ptr<FlashPressableDelegate>());

private:
    struct Impl;
};

#endif /* defined(__Engine_Mac__FlashButtonDelegate__) */
