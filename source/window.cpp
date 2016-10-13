#include "window.h"

Window::Window(sf::VideoMode mode, std::string& title, sf::Uint32 style = sf::Style::Default ) {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.attributeFlags = sf::ContextSettings::Attribute::Core;
	settings.majorVersion = 4;
	settings.minorVersion = 3;

	m_sfWindow.create(mode, title, style, settings);
}

Window::~Window() {}

void Window::SetVsync(bool enabled) {
	m_sfWindow.setVerticalSyncEnabled(enabled);
}

bool Window::IsOpen() {
	return m_sfWindow.isOpen();
}

void Window::Display() {
	m_sfWindow.display();
}