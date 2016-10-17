#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	Functions that help to access Lua scripts.
*/

#include <vector>

struct lua_State;

namespace luahelp {
	/*!
	  \brief This will load a Lua script into the lua state.
	  \param L Pointer to the current Lua state.
	  \param filename Name of the file to be loaded.
	  \return True or fales depending on if the file loaded correctly.
	*/
	bool LoadScript(lua_State* L, const std::string& filename);

	/*!
	  \brief This will get to the Lua stack.
	  \param L Pointer to the current Lua state.
	  \param variableName Name of the variable on the Lua stack to look for.
	*/
	void LuaGetToStack(lua_State* L, const std::string& variableName);

	/*!
	  \brief This will load the keys from the current lua script.
	  \param L Pointer to the current Lua State.
	*/
	void LoadGetKeysFunction(lua_State* L);

	/*!
	  \brief Returns a vector of strings containing the names of all the keys.
	  \param L Pointer to the current Lua state.
	  \param name Name of the scripts to get the keys from.
	  \return Vector of strings containing the key names.
	*/
	std::vector<std::string> GetTableKeys(lua_State* L, const std::string& name);
}