#pragma once

/**
* @Author	DJ Coombes
* @date		08th December 1016
* @brief	Used as a generic manager for SFML resources.
*/

#include <string>
#include <unordered_map>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "logger.h"

template <typename Resource>
class ResourceManager {
public:
	/*!
	  \brief Constructor.
	*/
	ResourceManager() {

	}

	/*!
	  \brief Destructor,
	*/
	~ResourceManager() {

	}

	/*!
	  \brief Load an sfml resource on to the resource map.
	  \param id ID to be used to reference the resource.
	  \param filename Name of the file to load.
	  \return True if the resource loaded successfully, false otherwise.
	*/
	bool LoadResource(const std::string& id, const std::string& filename) {
		Resource resource;

		if (!resource.loadFromFile(filename)) {
			LOG(ERRORR) << "Resource not found: " << filename;
			return false;
		}

		m_resources[id] = resource;
		return true;
	}

	/*!
	\brief Load an sfml resource on to the resource map.
	\param id ID to be used to reference the resource.
	\param filename Name of the file to load.
	\param shaderParam Parameter used for shader resources only.
	\return True if the resource loaded successfully, false otherwise.
	*/
	template <typename ShaderParam>
	bool LoadResource(const std::string& id, const std::string& filename, const ShaderParam& shaderParam) {
		Resource resource;

		if (!resource.loadFromFile(filename, shaderParam)) {
			LOG(ERRORR) << "Resource not found: " << filename;
			return false;
		}

		m_resources[id] = resource;
		return true;
	}

	/*!
	  \brief Return a reference to a resource.
	  \param id ID of the resource to get.
	  \return Reference to the resource.
	*/
	Resource& GetRef(const std::string& id) {
		auto resource = m_resources.find(id);
		if (resource == m_resources.end()) {
			throw std::runtime_error("Resource " + id + " not found!");
			LOG(ERRORR) << "Resource " << id << " not found.";
		}
		return resource;
	}

	/*!
	  \brief Clear the resource manager of all stored resources.
	*/
	void Clear() {
		m_resources.clear();
	}

private:
	std::unordered_map<std::string, Resource> m_resources; //!< Unordered map of all resources with string ID.
};

using TextureManager = ResourceManager<sf::Texture>;
using ShaderManager = ResourceManager<sf::Shader>;
using SoundManager = ResourceManager<sf::SoundBuffer>;