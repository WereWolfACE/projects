#ifndef _CORE_ANDROID_APPLICATION_H_
#define _CORE_ANDROID_APPLICATION_H_

#pragma once

namespace Core
{

class EngineAppDelegate;

int RunApplicationWithDelegate(void *vm, void *reserved, EngineAppDelegate *dlgt);

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

private:
	// Это нужно для того, чтобы иметь возможность запаузить тред.
	bool m_pause;
	boost::mutex m_pause_mutex;
	boost::condition_variable m_pause_changed;
};

}

void ShowKeyboard();
void ShowNumericKeyboard();
void HideKeyboard();

#endif // _CORE_ANDROID_APPLICATION_H_
