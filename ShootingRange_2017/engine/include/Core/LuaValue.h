#pragma once
#include "LuaBind.h"
#include "Script.h"

namespace Core
{
	class LuaValueHolder;

	class LuaValue{
	public:
		LuaValue(const luabind::adl::object& object);
		LuaValue(const LuaValue& other);
		~LuaValue();

		const LuaValue& operator=(const LuaValue& other);

		operator luabind::adl::object();
		luabind::adl::object operator->();
		luabind::adl::object get();

		static unsigned int GetLiveCount();
	private:
		LuaValueHolder* holder;
	};
}