/**
* @Author	DJ Coombes
* @date	13th October 2016
* @brief	Wrapper class for the SFML context window.
*/

#include "window.h"

#include <iostream>

#include "logger.h"
#include "gl_core_4_4.hpp"

Window::Window(sf::VideoMode mode, const std::string& title, sf::Uint32 style = sf::Style::Default ) {
	sf::ContextSettings settings;
	settings.depthBits = 0;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 0;
	settings.attributeFlags = sf::ContextSettings::Attribute::Default;
	settings.majorVersion = 4;
	settings.minorVersion = 2;

	m_width = mode.width;
	m_height = mode.height;

	m_sfWindow.create(mode, title, style, settings);

	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();

	if (!didLoad) {
		LOG(ERRORR) << "GLLoadGen failed to load functions.";
	}

	LOG(INFO) << "Number of functions that failed to load: " << didLoad.GetNumMissing();
}

Window::~Window() {}

void Window::SetVsync(bool enabled) {
	m_sfWindow.setVerticalSyncEnabled(enabled);
}

bool Window::IsOpen() {
	return m_sfWindow.isOpen();
}

sf::RenderWindow& Window::GetWindow() {
	return m_sfWindow;
}

int Window::GetWidth() {
	return m_width;
}

int Window::GetHeight() {
	return m_height;
}

void Window::Display() {
	if (m_sfWindow.isOpen())
		m_sfWindow.display();
}