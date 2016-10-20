/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Functions that help to access Lua scripts.
*/

#include "luaHelperFunctions.h"

#include <iostream>
#include <string>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "logger.h"

bool luahelp::LoadScript(lua_State* L, const std::string& filename) {
	if (!(luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))) {
		return true;
	}
	else {
		LOG(ERRORR) << "Failed to load: " << filename;
		LOG(ERRORR) << lua_tostring(L, -1);
		lua_pop(L, 1); // pop the error message from stack
		return false;
	}
}

void luahelp::LuaGetToStack(lua_State* L, const std::string& variableName) {
	int level = 0;
	std::string name = "";
	for (unsigned int i = 0; i < variableName.size(); i++) {
		if (variableName.at(i) == '.') {
			if (level == 0)
				lua_getglobal(L, name.c_str());
			else
				lua_getfield(L, -1, name.c_str());

			if (lua_isnil(L, -1)) {
				LOG(WARNING) << "Can't get: " << variableName;
				return;
			}
			else {
				name = "";
				++level;
			}
		}
		else {
			name += variableName.at(i);
		}
	}

	if (level == 0) {
		lua_getglobal(L, name.c_str());
	}
	else {
		lua_getfield(L, -1, name.c_str());
	}

	// remove other tables
	for (int i = 0; i < level; ++i) {
		lua_remove(L, -2);
	}
}

void luahelp::LoadGetKeysFunction(lua_State* L) {
	std::string code =
		R"(function getKeys(t)
        s = {}
        for k, v in pairs(t) do 
            table.insert(s, k)
            end 
        return s 
        end)";
	luaL_dostring(L, code.c_str());
}

std::vector<std::string> luahelp::GetTableKeys(lua_State* L, const std::string& name) {
	lua_getglobal(L, "getKeys"); // get function
	if (lua_isnil(L, -1)) {
		LOG(WARNING) << "Get keys function is not loaded. Loading...";
		LoadGetKeysFunction(L);
		lua_getglobal(L, "getKeys");
	}

	LuaGetToStack(L, name);

	lua_pcall(L, 1, 1, 0); // execute function. Got table on stack

	lua_pushnil(L);

	std::vector<std::string> keys;

	while (lua_next(L, -2)) { // get values one by one
		if (lua_isstring(L, -1)) {
			keys.push_back(lua_tostring(L, -1));
		}
		lua_pop(L, 1);
	}

	lua_settop(L, 0); // remove s table from stack 
	return keys;
}