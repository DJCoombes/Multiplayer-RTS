/**
* @Author	DJ Coombes
* @date		27th February 2017
* @brief	The state system handles the updating and changing of Lua states for each of the entities.
*/


#include "systemState.h"

#include "componentState.h"

SystemState::SystemState(SharedContext* context) : m_sharedContext(context) {

}

void SystemState::Update(EntityContainer& entities, float timeStep) {
	lua_State* L = m_sharedContext->m_entityManager->GetLuaState();
	try {
		luaL_dofile(L, "./resources/scripts/stateManager.lua");
		lua_pcall(L, 0, 0, 0);

		luabridge::LuaRef updateScript = luabridge::getGlobal(L, "CallUpdate");
		updateScript(entities.size());
	}
	catch (luabridge::LuaException const& e) {
		LOG(ERRORR) << "[LUA] luaException " << e.what();
	}
}

void SystemState::Draw(EntityContainer& entities) {}

void SystemState::HandleEvent(const EntityID& entity, const EntityEvent& event) {}

void SystemState::Notify(const Message& message) {}