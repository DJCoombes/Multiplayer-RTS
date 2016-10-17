/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Functions that help to access Lua scripts.
*/

#include "luaHelperFunctions.h"

#include <string>
#include <iostream>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

bool luahelp::LoadScript(lua_State* L, const std::string& filename) {
	if (!(luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))) {
		return true;
	}
	else {
		std::cout << "[C++] ERROR: Failed to load: " << filename << std::endl;
		std::cout << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return false;
	}
}

void luahelp::LuaGetToStack(lua_State* L, const std::string& variableName) {
	int level = 0;
	std::string name;
	for (int i : variableName) {
		if (i == '.') {
			if (level == 0)
				lua_getglobal(L, name.c_str());
			else
				lua_getfield(L, -1, name.c_str());

			if (lua_isnil(L, -1)) {
				std::cout << "[C++] Error: Can't get: " << variableName << std::endl;
				return;
			}
			else {
				name = "";
				level++;
			}
		}
		else {
			name += i;
		}
	}

	if (level == 0) {
		lua_getglobal(L, name.c_str());
	}
	else {
		lua_getfield(L, -1, name.c_str());
	}

	for (int i = 0; i < level; i++) {
		lua_remove(L, -2);
	}
}

void luahelp::LoadGetKeysFunction(lua_State* L) {
	std::string script =
		R"(function getKeys(T)
			s = {}
			for k, v in pairs(t) do
				table.insert(s, k)
				end
			return s
			end)";
	luaL_dostring(L, script.c_str());
}

std::vector<std::string> luahelp::GetTableKeys(lua_State* L, const std::string& name) {
	lua_getglobal(L, "getKeys");
	if (lua_isnil(L, -1)) {
		std::cout << "Get keys function is not loaded. Loading..." << std::endl;
		LoadGetKeysFunction(L);
		lua_getglobal(L, "getkeys");
	}

	LuaGetToStack(L, name);

	lua_pcall(L, 1, 1, 0);

	lua_pushnil(L);

	std::vector<std::string> keys;

	while (lua_next(L, -2)) {
		if (lua_isstring(L, -1)) {
			keys.push_back(lua_tostring(L, -1));
		}
		lua_pop(L, 1);
	}

	lua_settop(L, 0);
	return keys;
}