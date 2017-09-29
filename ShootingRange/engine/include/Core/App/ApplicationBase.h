#ifndef __CORE_APPLICATIONBASE_H__
#define __CORE_APPLICATIONBASE_H__

#pragma once

#include "Core/Timer.h"


namespace Core
{

enum class SleepPolicy
{
	Allow, // win, android - sleep используется для ограничения fps
	Disallow // ios, mac - ограничение fps делается без sleep
};

// Класс для автоматического управления объектом таймера
class StopTimerGuard {
public:
	explicit StopTimerGuard(Timer& timer) : _timer(&timer) {
		_timer->Pause();
	}

	~StopTimerGuard() {
		if (_timer)
			_timer->Resume();
	}

	StopTimerGuard(const StopTimerGuard&) = delete;
	StopTimerGuard& operator=(const StopTimerGuard&) = delete;

	StopTimerGuard(StopTimerGuard&& rhs) noexcept {
		_timer = rhs._timer;
		rhs._timer = nullptr;
	}

	StopTimerGuard& operator=(StopTimerGuard&& rhs) noexcept {
		_timer = rhs._timer;
		rhs._timer = nullptr;
		return *this;
	}

private:
	Timer* _timer;
};

class Application;

class ApplicationBase {
public:
	ApplicationBase();

	virtual ~ApplicationBase();

	// Вызывается, когда загружена очередная порция ресурсов
	void UpdateLoadScreen();

	// Нарисовать один кадр
	void MainLoopContent();

	// вызвать задержку, чтобы ограничить fps до запланированного значения (для андроида и win)
	void WaitForFixedFpsOrLoadResources(SleepPolicy sleepPolicy);

	void Draw();

	// чистка ресурсов
	void Shutdown();

	// Вернуть текущий fps
	int GetCurrentFps() const { return _currentFps; }

	void SetPause(bool paused);
	bool IsPaused() const { return _isPaused; }

	// Сказать, что приложение загружено
	void SetLoaded() { _loading = false; }

	bool IsLoading() const { return _loading; } // устанавливается из игры, может устанавливаться после проверки доступности сервера и т.д.

	bool AllResourcesLoaded() const; // формальная проверка загрузчика ресурсов

	uint32_t GetResourceCounter() const { return _resourceCounter; }
	uint32_t GetLoadingCounter() const { return _loadingCounter; }

	// Выполнить функцию в главном потоке
	// Вызывающий поток планирует вызов функции и не блокируется на этом вызове
	void RunInMainThread(boost::function<void()> func);
	// Выполнить функцию в главном потоке
	// Вызывающий поток будет блокирован на этом вызове до выполнения функции основным потоком
	void RunInMainThreadAndWait(boost::function<void()> func);

	boost::asio::io_service& GetMainIoService() { return _io_service; }

	void UpdateMessageQueue();
	void UpdateAsyncWorkingQueue();

	// Дождаться загрузки запланированных ресурсов
	void WaitLoading();

	void RunMappedCommand(int key);

	// Временно останавливает игровой таймер
	//
	// Типичное использование:
	//    {
	//       auto guard = appInstance->PauseTimer();
	//       ... long operation
	//    }
	StopTimerGuard PauseTimer() {
		StopTimerGuard guard{_timer};
		return guard;
	}

	void RegisterExitTask(const std::string& name, boost::function<void()> task);
	void UnregisterExitTask(const std::string& name);
	void UnregisterAllExitTasks();
	bool IsExitTaskRegistered(const std::string& name) const;
	size_t GetExitTasksCount() const;

	virtual void BeginExitTasksExecution();
	virtual void FinishExitTasksExecution();

protected:
	void InitInternal();

	// регистрация lua-типов
	virtual void ScriptMap();
	// регистрация классов в фабрике
	virtual void RegisterTypes();

private:
	void LoadKeyMap();

protected:
	Timer _timer;
	int _currentFps;
	int _fpsCounter;
	float _fpsTimer;
	uint32_t _resourceCounter;
	uint32_t _loadingCounter;

	std::string _keyMap[91];

	mutable boost::mutex _tasks_mutex;

	typedef std::map<std::string, boost::function<void()>> Tasks;
	Tasks _tasks;

private:
	bool _isPaused;
	bool _loading;
		// Загружаемся ли;
		// этот флаг нужен во время загрузки, чтобы:
		// * игнорировать паузу;
		// * жестко выходить по требованию пользователя.

	float _dt_real;
	float _sleep_time;

	boost::asio::io_service _io_service;
};

extern Application *appInstance;

} // namespace Core

#endif // __CORE_APPLICATIONBASE_H__
