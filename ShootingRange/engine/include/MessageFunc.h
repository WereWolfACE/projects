#ifndef __MESSAGEFUNC_H__
#define __MESSAGEFUNC_H__

#pragma once

#include "Core/MessageManager.h"
#include "Core/Screen.h"
#include "Core/Script.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

class MessageFunc : public RefCounter
{
	friend class Layer;

	Layer *_bindLayer;

public:
	MessageFunc();

    Layer* getThisLayer();

	virtual void AcceptMessage(const Message& message);

	static MessageFunc* Create(const std::string& className);
};

typedef boost::intrusive_ptr<MessageFunc> MessageFuncPtr;

class LuaHandlerFunc : public MessageFunc
{
	std::string _functionName;

public:
	LuaHandlerFunc(const std::string& functionName);

	virtual void AcceptMessage(const Message& message);

};

#endif // __MESSAGEFUNC_H__
