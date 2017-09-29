#ifndef __CONTROLLERMANAGER_H__
#define __CONTROLLERMANAGER_H__

#pragma once

#include "GUI/Widget.h"
#include "RefCounter.h"

class IController : public RefCounter
{
public:
	IController();
	IController(const std::string& name);

	float local_time;
	bool finished;
	bool paused;

	const std::string& getName() const;

	virtual void Update(float dt) = 0;
	virtual bool isFinish() = 0;

private:
	std::string _name;
};

typedef boost::intrusive_ptr<IController> IControllerPtr;

namespace Core {

class ControllerManager 
{
public:
	ControllerManager();
	~ControllerManager();

	void Update(float dt);
	void KillControllers(const std::string& name);
	void Clear();
	void PauseControllers(const std::string& name);
	void ContinueControllers(const std::string& name);
	void addController(IController* controller);
	void addQueuedControllers();
	void UpdateAdded(float dt);
	bool hasController(const std::string& name);

private:
	void EraseFinished();

private:
	friend class ScreenImpl;

	typedef std::list<IControllerPtr> Controllers;
	
	Controllers _controllers;
	Controllers _addedControllers;

	bool isAdded;
};

extern ControllerManager controllerKernel;

} // namespace Core

#endif // __CONTROLLERMANAGER_H__
