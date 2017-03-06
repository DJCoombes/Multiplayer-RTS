/**
* @Author	DJ Coombes
* @date		8th January 2017
* @brief	Engine class, used for all engine events.
*/

#include "keyboard.h"

#include <fstream>
#include <sstream>

Keyboard::Keyboard() {
	LoadKeyBinds();
}

Keyboard::~Keyboard() {

}

void Keyboard::HandleEvent(sf::Event& event) {
	if (event.type == sf::Event::KeyReleased) {
		CallBind(event.text.unicode);
	}
}

void Keyboard::UnbindKey(char key) {
	auto bind = m_bindings.find(key);
	if (bind == m_bindings.end())
		return;
	m_bindings.erase(bind);
}

void Keyboard::LoadKeyBinds() {
	std::ifstream bindingsFile(CONFIGLOCATION);
	if (bindingsFile.good()) {
		bindingsFile >> m_bindingsJson;
		bindingsFile.close();
	}
	else {
		LOG(INFO) << "Key config file not found. Createing default one...";
		CreateDefaulBindingsFile();
	}
}

void Keyboard::CreateDefaulBindingsFile() {
	m_bindingsJson["forward"]		= "w";
	m_bindingsJson["backwards"]		= "s";
	m_bindingsJson["left"]			= "d";
	m_bindingsJson["right"]			= "a";
	m_bindingsJson["changeView"]	= "k";

	std::ofstream keyBinds;
	keyBinds.open(CONFIGLOCATION, std::ios::out);
	keyBinds << std::setw(4) << m_bindingsJson << std::endl;
	keyBinds.close();
	LOG(INFO) << "Finished creating default key config file.";
}

void Keyboard::CallBind(char key) {
	auto bind = m_bindings.find(key);
	if (bind == m_bindings.end())
		return;
	bind->second.second();
}