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
	// Try to load the lua file onto the Lua state.
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

void luahelp::GetToLuaStack(lua_State* L, const std::string& variableName) {
	int level = 0;
	std::string name = "";
	// Try to find the area on the stack the variable is located.
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
	// Get the global variable.
	if (level == 0) {
		lua_getglobal(L, name.c_str());
	}
	// Get the variable from a table.
	else {
		lua_getfield(L, -1, name.c_str());
	}

	// remove other tables
	for (int i = 0; i < level; ++i) {
		lua_remove(L, -2);
	}
}

void luahelp::GetLuaKeys(lua_State* L) {
	// This function is used to get the Lua keys from the Lua stack, the function will be loaded onto the Lua state.
	std::string code =
		R"(function getKeys(t)
        s = {}
        for k, v in pairs(t) do 
            table.insert(s, k)
            end 
        return s 
        end)";
	// Try to call the Lua function.
	try {
		luaL_dostring(L, code.c_str());
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
}

std::vector<std::string> luahelp::GetTableKeys(lua_State* L, const std::string& name) {
	// Call the Lua get keys function from the Lua state.
	lua_getglobal(L, "getKeys");
	// Check if the function is loaded, if not then load the function.
	if (lua_isnil(L, -1)) {
		LOG(WARNING) << "Get keys function is not loaded. Loading...";
		GetLuaKeys(L);
		lua_getglobal(L, "getKeys");
	}
	// Find the place on the stack that the variable is located.
	GetToLuaStack(L, name);
	// This will call the Lua function.
	lua_pcall(L, 1, 1, 0);
	// This will push a nil value onto the stack.
	lua_pushnil(L);

	std::vector<std::string> keys;
	// Try to get each of the keys in the Lua table.
	try {
		while (lua_next(L, -2)) {
			if (lua_isstring(L, -1)) {
				keys.push_back(lua_tostring(L, -1));
			}
			lua_pop(L, 1);
		}
	}
	catch (const std::exception& e) {
		LOG(DEBUG) << e.what();
	}
	// Clean the Lua stack before exiting.
	lua_settop(L, 0);
	return keys;
}