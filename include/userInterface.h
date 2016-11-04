#pragma once

/**
* @Author	DJ Coombes
* @date		25th October 2016
* @brief	The user interface class will handle the Awesomium binding, including updating and drawing.
*/

#include <iostream>

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/MethodDispatcher.h>

#include "window.h"
#include "inputForwarder.h"

typedef unsigned int GLuint;

class UserInterface {
public:
	/*!
	  \brief Constructor.
	  \param window Reference to the context window.
	*/
	UserInterface(Window& window);

	/*!
	  \brief Destructor.
	*/
	~UserInterface();

	/*!
	  \brief Initialize everything to do with the user interface.
	  \return True if the UI was initialized successfully, false otherwise.
	*/
	bool InitializeUI();

	/*!
	  \brief Pass an SFML event to the web renderer.
	  \param event The SFML event to pass.
	*/
	void HandleEvent(sf::Event& event);

	/*!
	  \brief Update the web view.
	*/
	void UpdateView();

	/*!
	  \brief Draw the user interface to the context window.
	*/
	void DrawUI();

	/*!
	  \brief Resize the web view based on the new size.
	*/
	void Resize();

	/*!
	  \brief Get a pointer to the current web view.
	  \return Pointer to the web view.
	*/
	Awesomium::WebView* GetWebView();

	/*!
	  \brief Call a JavaScript function from C++.
	  \param funcName Name of the function to call.
	  \param args A JavaScript array containing the function arguments.
	*/
	void CallJSFunc(Awesomium::WebString& funcName, Awesomium::JSArray args);

	/*!
	  \brief Bind a C++ function to be called from JavaScript.
	  \param methodName Name of the method which is used in JavaScript the call the function.
	  \param method Function pointer to the function.
	  \param instance Instance of the object from which the function is to be called.
	*/
	template<class T>
	void BindMethod(const Awesomium::WebString& methodName, void (T::*method)(Awesomium::WebView*, const Awesomium::JSArray&), T* instance){
		if (m_result.IsObject()) {
			m_methodDispatcher.Bind(m_engineObject, methodName, JSDelegate(instance, method));
		}
	}

	/*!
	  \brief Unbind a method so it can no longer be called from JavaScript.
	  \param name Name of the method.
	*/
	void UnBindMethod(const std::string& name);
private:
	/*!
	  \brief Function to call the logger from JavaScript.
	  \param caller The web view that's calling the function.
	  \param args The arguments passed from the web view.
	*/
	void WebLog(Awesomium::WebView* caller, const Awesomium::JSArray& args);

	Window&						m_window; //!< Reference to the window context.

	Awesomium::WebCore*			m_webCore; //!< Pointer to the web engine.
	Awesomium::WebView*			m_webView; //!< Pointer to the web view.
	Awesomium::BitmapSurface*	m_surface; //!< Pointer to the render surface.

	InputForwarder				m_inputForwarder; //!< Object used to pass events to the web renderer.

	MethodDispatcher			m_methodDispatcher; //!< The method dispatcher dispatches JavaScript methods.
	Awesomium::JSValue			m_result; //!< Global JavaScript object from which to call C++ functions.
	Awesomium::JSValue			m_webWindow; //!< Object from which to call JavaScript functions from C++.
	Awesomium::JSObject			m_engineObject; //!< Used to bind C++ methods.

	GLuint m_shaderProgram; //!< The shader program containing the shaders.
	GLuint m_VAO; //!< Vertex array object.
	GLuint m_VBO; //!< Vertex buffer object.
	GLuint m_EBO; //!< Element buffer object.
};