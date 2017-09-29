#ifndef __LUASTACKGUARD_H__
#define __LUASTACKGUARD_H__

#pragma once

#include "Core/Script.h"
#include "EngineAssert.h"

namespace Core {

class LuaStackGuard {
public:
	LuaStackGuard()
		: _strict(false)
	{
		_old_top = lua_gettop(luaState);
	}

	explicit LuaStackGuard(bool strict)
		: _strict(strict)
	{
		_old_top = lua_gettop(luaState);
	}

	~LuaStackGuard() {
		if (_strict) {
			int new_top = lua_gettop(luaState);
			Assert(new_top == _old_top);
		}

		lua_settop(luaState, _old_top);
	}

private:
	LuaStackGuard(const LuaStackGuard&);
	LuaStackGuard& operator=(const LuaStackGuard&);

private:
	bool _strict;
	int _old_top;
};

} // namespace Core

#endif // __LUASTACKGUARD_H__
