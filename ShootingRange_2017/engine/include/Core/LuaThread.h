#ifndef _CORE_LUATHREAD_H_
#define _CORE_LUATHREAD_H_

/**
 * Классы для запуска сценариев на Lua в режиме co-routine,
 * т.е. можно приостанавливать их выполнение.
 * 
 * Это родная фича языка Lua, подробнее можно читать тут:
 * http://lua-users.org/wiki/CoroutinesTutorial
 */

#include <deque>
#include "boost/shared_ptr.hpp"
#include "Core/Script.h"
#include "Core/TimeMachine.h"
#include "EngineAssert.h"

//----------------------------------------------------------------------

class LuaThread;


class LuaThreadManager
{
protected:
	typedef boost::shared_ptr<LuaThread> LuaThread_HardPtr;
	typedef std::deque<LuaThread_HardPtr> Items;
	Items _items;
	bool _paused;
public:
	LuaThreadManager() : _paused(false) {}
	~LuaThreadManager() {}

	/// Добавленный тред будет удалён, когда завершит свою работу.
	void AddThread(LuaThread_HardPtr thread);

	/// Убирает тред из списка и возвращает указатель на него.
	/// Если такой тред не найден, возвращает NULL.
	LuaThread_HardPtr RemoveThread(LuaThread * thread);

	LuaThread_HardPtr FindThread(LuaThread * thread);

	bool isEmpty() {
		SCRIPT_THREAD_LOCK;
		return _items.empty();
	}

	void Update(float dt);

	void TerminateAll();
	void DetachAll();
	
	void Pause() { _paused = true; }
	void Unpause() { _paused = false; }

	friend std::ostream& operator<<(std::ostream &os, const LuaThreadManager& tm);
};

namespace Core {
	extern LuaThreadManager luaThreadManager;
}

//----------------------------------------------------------------------

class LuaThread
{
public:
	typedef boost::shared_ptr<LuaThread> HardPtr;

protected:
	lua_State* _thread;
	int _refKey; // индекс объекта треда, хранящегося в lua registry.

	float _wait; // Время паузы
	float _t; // Таймер ожидания

	bool _finished; // Функция отработала.
	bool _running; // Функция выполняется.
	bool _paused;

	// Пояснение по жизненному циклу треда:
	// В самом начале _finished = false; _running = false, т.е. тред не выполняется, но ещё и не закончился, потому что он ещё будет выполняться.
	// При первом апдейте проверяется, что _running == false, и тогда тред запускается, после чего _running == true, _finished == false.
	// Когда выполнение функции завершается, то _running = true и _finished = true.
	// После завершения тред удаляется из менеджера тредов.

	luabind::object _func; // Функция, которая будет запущена.

	LuaThread * _parent;
	LuaThreadManager _children;
	
	Core::TimeMachine _timeMachine;

#ifdef _DEBUG
	std::string _debugInfo;
#endif

private:
	LuaThread(const LuaThread&); // non-copyable!
	LuaThread& operator=(const LuaThread&);

public:
	LuaThread()
		: _thread()
		, _refKey()
		, _wait(0.f)
		, _t(0.f)
		, _finished(false)
		, _running(false)
		, _paused(false)
		, _parent()
	{
		Init();
	}

	~LuaThread();

	void SetFunc(const luabind::object& func);
	void SetTable(const luabind::object& table);
	
	void DoJob(Core::JobBase* job);

	/// Вызывается изнутри lua-функции, чтобы эмулировать паузу.
	/// В lua возвращает время, прошедшее с предыдущего вызова Wait.
	/// Для установки паузы не текущему треду используйте SetWait.
	void Wait(float delay);

	/// Задержка с учётом запаздывания с предыдущего раза.
	/// Используется, когда нужно, чтобы ошибка не накапливалась.
	void WaitAdd(float delay);

	/// Вызов SetWait(t) установит ожидание в t секунд от текущего момента.
	/// Не прерывает выполнение текущего треда в lua, в отличие от Wait.
	void SetWait(float delay);
	void ResetWait(); /// Сброс времени ожидания.

	/// Вызывается в игровом цикле.
	/// Внимание: иногда dt может быть нулевым!
	void Update(float dt);

	bool isFinished() {
		SCRIPT_THREAD_LOCK;
		return _finished && !hasChildren();
	}

	bool hasChildren() {
		SCRIPT_THREAD_LOCK;
		return !_children.isEmpty();
	}

	/// Поставить на паузу. Пауза начнётся со следующего вызова Wait.
	void Pause() { _paused = true; }
	void Unpause() { _paused = false; }
	void PauseChildren() { _children.Pause(); }
	void UnpauseChildren() { _children.Unpause(); }

	/// Остановить, несмотря ни на что.
	void Terminate();

	/// Прибить все дочерние потоки.
	void TerminateChildren();

	/// Присоединить тред
	void Attach(LuaThread::HardPtr thread);
	// Внимание! Не присоединяйте тред по указателю LuaThread *.
	// Например, нельзя в луа писать t:Attach(Thread()).
	void Attach(LuaThread * thread) { Assert(false); } //~ Так делать нельзя.
	// У вас должен быть HardPtr, чтобы быть уверенными, что объект не был удалён к этому моменту.

	/// Переместить тред в глобальный менеджер тредов.
	/// Возвращает ссылку на тред, её можно использовать,
	/// чтобы присоединить тред к другому родителю.
	LuaThread::HardPtr Detach();

	/// Отсоединить все дочерние треды.
	void DetachChildren();

	/// Получение объекта LuaThread для текущего lua-треда.
	static LuaThread * GetCurrent();
	LuaThread::HardPtr GetPtr();

	/// Получение локальной таблицы для треда.
	luabind::object Local() const;

	friend std::ostream& operator<<(std::ostream &os, const LuaThread& t);
	friend std::ostream& operator<<(std::ostream &os, const LuaThreadManager& tm);
protected:
	/// Вспомогательная функция для получения текущего thread environment.
	static luabind::object ThreadEnv();

	void Init(); // Создать тред.
	void Destroy(); // Уничтожение треда.

	void OnStop(); // Вызывается при остановке треда, запускает OnStopFunc.
};

//----------------------------------------------------------------------

LuaThread::HardPtr SpawnLuaThread(luabind::object func, luabind::object table);

inline void RunLuaThread(luabind::object func, luabind::object table)
{
	SCRIPT_THREAD_LOCK;
	LuaThread::HardPtr t = SpawnLuaThread(func, table);
	if (t == NULL) { Assert(false); return; }
	t->Update(0.f);
}

inline LuaThread::HardPtr SpawnLuaThread(const std::string& func, luabind::object table = luabind::object())
{
	return SpawnLuaThread(luabind::globals(Core::luaStateMain)[func], table);
}

inline void RunLuaThread(const std::string& func, luabind::object table = luabind::object())
{
	RunLuaThread(luabind::globals(Core::luaStateMain)[func], table);
}

inline LuaThread::HardPtr SpawnLuaThread(luabind::object func)
{
	return SpawnLuaThread(func, luabind::object());
}

inline void RunLuaThread(luabind::object func)
{
	RunLuaThread(func, luabind::object());
}

//~ #define nil __null

#endif
