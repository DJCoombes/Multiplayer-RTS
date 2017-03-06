#pragma once

/**
* @Author	DJ Coombes
* @date		8th January 2017
* @brief	Keyboard class, used for handling key input.
*/

#include <unordered_map>
#include <utility>
#include <functional>
#include <string>

#include <json.hpp>
#include <SFML/Window.hpp>

#include "logger.h"

#define CONFIGLOCATION "resources/configs/keybindings.json"

using Json = nlohmann::json;
using Binds = std::unordered_map<char, std::pair<std::string, std::function<void()>>>;

class Keyboard {
public:
	/*!
	  \brief Constructor.
	*/
	Keyboard();

	/*!
	  \brief Destructor.
	*/
	~Keyboard();

	void HandleEvent(sf::Event& event);

	/*!
	  \brief Bind a function to a key based on the name associated to the key.
	  \param T Type of object.
	  \param name Name related to the key bind to search for.
	  \param method Pointer to the function which is to be called when the key associated to it is pressed.
	  \param instance Instance of the object from which to call the function.
	*/
	template<class T>
	void BindKey(const std::string& name, void (T::*method)(), T* instance) {
		int key = m_bindingsJson[name];
		auto bind = m_bindings.find(key);
		auto temp = std::bind(method, instance);
		auto pair = std::make_pair(name, temp);
		m_bindings.emplace(key, pair);
	}

	/*!
	  \brief Remove a currently bound key.
	  \param key The key to unbind.
	*/
	void UnbindKey(char key);

	/*!
	  \brief Load the key bindings from the json config file.
	*/
	void LoadKeyBinds();

	/*!
	  \brief Create the default key bindings file.
	*/
	void CreateDefaulBindingsFile();

	/*!
	  \brief Call a specific key bind.
	  \param key The key associated with the bind.
	*/
	void CallBind(char key);
private:
	Binds	m_bindings; //!< Structure used to store the currently bound keys and functions.
	Json	m_bindingsJson; //!< Json file with the char to the name of the key bind.
	std::unordered_map<int, int> m_sfmlToInt; //!< Unordered map used to convert SFML key codes to native char values.
};