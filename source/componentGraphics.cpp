#include "componentGraphics.h"

#include <LuaBridge.h>

#include "logger.h"

ComponentGraphics::ComponentGraphics(luabridge::LuaRef& componentTable) {
	auto nameTemp = componentTable["name"];

	if (nameTemp.isString())
		name = nameTemp.cast<std::string>();
	else
		LOG(ERRORR) << "ComponentGraphics.name is not a string!";
}

std::shared_ptr<ComponentBase> ComponentGraphics::Clone() const {
	return std::make_shared<ComponentGraphics>(*this);
}