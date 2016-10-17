#pragma once

/**
* @Author	DJ Coombes
* @date		17th October 2016
* @brief	The entity class is a continer for components. Also stores generic information.
*/

#include <string>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <memory>

#include "componentBase.h"

using EntityID = unsigned int;

class Entity {
public:

private:
	std::string		m_name;
	std::string		m_type;
	EntityID		m_id;

	std::map<std::type_index, std::shared_ptr<ComponentBase>> m_components;
};