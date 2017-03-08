/**
* @Author	DJ Coombes
* @date		07th March 2017
* @brief	The external Lua API functions are stored here.
*/

#include "luaFunctions.h"

#include "entityManager.h"

#include <LuaBridge.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

EntityManager* LuaFuncs::manager;

void LuaFuncs::AddFunctions() {
	luabridge::getGlobalNamespace(manager->GetLuaState())
		.addFunction("Log", LuaFuncs::Log)
		.addFunction("GetEntity", LuaFuncs::GetEntity)
		.addFunction("Create", LuaFuncs::Create)
		.addFunction("Destroy", LuaFuncs::Destroy)
		.addFunction("Damage", LuaFuncs::Damage)
		.addFunction("HasStateComponent", LuaFuncs::HasStateComponent)
		.addFunction("GetStateType", LuaFuncs::GetStateType)
		.addFunction("GetState", LuaFuncs::GetState)
		.addFunction("SetState", LuaFuncs::SetState)
		.addFunction("SetStartState", LuaFuncs::SetStartState)
		.addFunction("GetStarting", LuaFuncs::GetStarting);
}

void LuaFuncs::Log(const std::string& message) {
	LOG(INFO) << message;
}

std::shared_ptr<Entity> LuaFuncs::GetEntity(int id) {
	return manager->GetEntity(id);
}

int LuaFuncs::Create(const std::string& type, int id) {
	return manager->Create(type);
}

void LuaFuncs::Destroy(int id) {
	manager->Destroy(id);
}

void LuaFuncs::Damage(int id, int secondID) {

}

bool LuaFuncs::HasStateComponent(int id) {
	auto entity = GetEntity(id);
	auto sc = entity->Get<ComponentState>();
	if (sc == nullptr)
		return false;
	return true;
}

std::string LuaFuncs::GetStateType(int id) {
	auto entity = GetEntity(id);
	auto sc = entity->Get<ComponentState>();
	return sc->m_stateMachineType;
}

std::string LuaFuncs::GetState(int id) {
	auto entity = GetEntity(id);
	auto sc = entity->Get<ComponentState>();
	return sc->m_state;
}

void LuaFuncs::SetState(int id, std::string state) {
	auto entity = GetEntity(id);
	auto sc = entity->Get<ComponentState>();
	sc->m_state = state;
}

void LuaFuncs::SetStartState(int id, bool starting) {
	auto entity = GetEntity(id);
	auto sc = entity->Get<ComponentState>();
	sc->m_start = starting;
}

bool LuaFuncs::GetStarting(int id) {
	auto entity = GetEntity(id);
	auto sc = entity->Get<ComponentState>();
	return sc->m_start;
}