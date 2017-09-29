#ifndef _CORE_SCRIPT_H_
#define _CORE_SCRIPT_H_

#include "LuaBind.h"
#include "ThreadSupport.h"
#include "Utils/utils.hpp"

#define SCRIPT_THREAD_LOCK MUTEX_LOCK_TYPE lock(*Core::luaMutex);

struct lua_State;
struct lua_Debug;

// основные функции для работы с Lua
namespace Core {

extern MUTEX_TYPE* luaMutex;
	
extern lua_State* luaState;
extern lua_State* luaStateMain;

void InitLua();

void LuaDoFile(const std::string& filename);
void LuaDoString(const std::string& str);
	
luabind::object LuaLoadFile(const std::string& filename);
luabind::object LuaLoadString(const std::string& str);
	
void LuaExecuteStartupScript(const std::string& filenameOrFuncOrCode);

void LuaClose(); // Close lua. Free all the memory and objects taken by lua.
void LuaCollectGarbage(); // Collects all garbage. May take relatively long time.
void LuaGCStep(); // Perform a garbage collection iteration. Used in the main loop.

int LuaErrorHandler(lua_State *L); ///< Вызывается при ошибках в Lua.
int LuaError(lua_State *L, const std::string& message = "");

std::string LuaToString(const luabind::object& obj); // Дамп переменной lua
std::string LuaDumpLocals(lua_State * L, lua_Debug * d); // Дамп всех локальных переменных
std::string LuaDumpStack(lua_State *L, bool dump_locals); // Возвращает call stack

void LuaStackTrace(lua_State *L = luaState); ///< Печатает call stack в консоль и в лог.
void LuaPrintStack(); ///< Вспомогательная функция для печати lua call stack.
void LuaPrintThreads(); ///< Напечатать всё дерево тредов.

void LuaStartDebugging();
void LuaStopDebugging();
	
void LuaBreak();

void LuaCallFunctionWaitThread(const char* funcName);
void LuaCallFunctionWaitThread(const luabind::object& obj);

//------------------------------------------------------------------

// Вспомогательный класс, обеспечивающий замену luaState
// на время текущей области видимости.
class TmpLuaStateChanger {
	lua_State* tmp;
private:
	TmpLuaStateChanger(const TmpLuaStateChanger&); // non-copyable!
	TmpLuaStateChanger& operator=(const TmpLuaStateChanger&);
public:
	TmpLuaStateChanger(lua_State* s) { tmp = luaState; luaState = s; }
	~TmpLuaStateChanger() { luaState = tmp; }
};

//------------------------------------------------------------------

template<class T>
inline T LuaCast(const luabind::object& obj) {
	try {
		return luabind::object_cast<T>(obj);
	}
	catch (luabind::cast_failed &e) {
		LuaError(e.state(), std::string("LUA: Cast error: ") + e.what());
	}
	return T();
}

//------------------------------------------------------------------

template <typename TRet, typename ...TArgs>
inline TRet LuaCallFunction(const char* funcName, TArgs&&... args)
{
	SCRIPT_THREAD_LOCK;
	try {
		return (TRet)luabind::call_function<TRet>(luaState, funcName, std::forward<TArgs>(args)...);
	}
	catch (luabind::error &e) {
		LuaError(e.state(), std::string("LUA: Error in call \"") + funcName +"\": "+ e.what());
	}
	return TRet();
}

template<typename TRet, typename ...TArgs>
inline TRet LuaCallFunction(const luabind::object& obj, TArgs&&... args)
{
	SCRIPT_THREAD_LOCK;
	try {
		if (obj.interpreter() != luaState) {
			return (TRet)luabind::call_function<TRet>(luabind::object(luaState, obj), std::forward<TArgs>(args)...);
		}
		else {
			return (TRet)luabind::call_function<TRet>(obj, std::forward<TArgs>(args)...);
		}
	}
	catch (luabind::error &e) {
		LuaError(e.state(), std::string("LUA: Error in call_function ") + utils::lexical_cast(obj) + ": " + e.what());
	}
	return TRet();
}

template <typename ...TArgs>
inline void LuaCallVoidFunction(const char* funcName, TArgs&&... args)
{
	LuaCallFunction<void>(funcName, std::forward<TArgs>(args)...);
}

//------------------------------------------------------------------
	
template<typename TRet, typename ...TArgs>
TRet LuaResumeFunction(const char* funcName, TArgs&&... args)
{
	SCRIPT_THREAD_LOCK;
	try {
		luabind::object fun = luabind::globals(luaState)[funcName];
		return (TRet)luabind::resume_function<TRet>(fun, std::forward<TArgs>(args)...);
	}
	catch (luabind::error &e) {
		LuaError(e.state(), std::string("LUA: Error in resume_function ") + funcName +": "+ e.what());
	}
	return TRet();
}

template<typename TRet, typename ...TArgs>
TRet LuaResumeFunction(const luabind::object& func, TArgs&&... args)
{
	SCRIPT_THREAD_LOCK;
	try {
		if (func.interpreter() != luaState) {
			return (TRet)luabind::resume_function<TRet>(luabind::object(luaState, func), std::forward<TArgs>(args)...);
		}
		else {
			return (TRet)luabind::resume_function<TRet>(func, std::forward<TArgs>(args)...);
		}
	}
	catch (luabind::error &e) {
		LuaError(e.state(), std::string("LUA: Error in resume_function ") + utils::lexical_cast(func) + ": " + e.what());
	}
	return TRet();
}

template <typename ...TArgs>
void LuaResumeVoidFunction(const char* funcName, TArgs&&... args)
{
	LuaResumeFunction<void>(funcName, std::forward<TArgs>(args)...);
}
	
template<typename TRet, typename ...TArgs>
TRet LuaResume(TArgs&&... args)
{
	SCRIPT_THREAD_LOCK;
	try {
		return (TRet)luabind::resume<TRet>(luaState, std::forward<TArgs>(args)...);
	}
	catch (luabind::error &e) {
		LuaError(e.state(), std::string("LUA: Error in resume: ") + e.what());
	}
	return TRet();
}

template <typename ...TArgs>
void LuaResumeVoid(TArgs&&... args) {
	LuaResume<void>(std::forward<TArgs>(args)...);
}

} // namespace Core

#endif // _CORE_SCRIPT_H_
