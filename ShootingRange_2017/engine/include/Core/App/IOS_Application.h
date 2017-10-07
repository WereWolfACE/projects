#ifndef _CORE_IOS_APPLICATION_H_
#define _CORE_IOS_APPLICATION_H_

#pragma once

namespace Core
{

class EngineAppDelegate;

void RunApplicationWithDelegate(int argc, char * argv[], EngineAppDelegate *dlgt);

class Application : public ApplicationBase {
public:
	Application();
	~Application();

	void Init();
	void Start();
	void Stop();

	void SetLoadingPause(bool pause);
	void BlockLoadingWhilePaused();

	bool IsFullscreen() const { return true; }

	virtual void BeginExitTasksExecution() override;
	virtual void FinishExitTasksExecution() override;

private:
	// Это нужно для того, чтобы иметь возможность запаузить тред.
	bool m_pause;
	boost::mutex m_pause_mutex;
	boost::condition_variable m_pause_changed;
};

}

#endif // _CORE_IOS_APPLICATION_H_
