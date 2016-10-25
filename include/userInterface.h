#pragma once

/**
* @Author	DJ Coombes
* @date		25th October 2016
* @brief	The user interface class will handle the Awesomium binding, including updating and drawing.
*/

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

#include "window.h"

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
private:
	Window&						m_window; //!< Reference to the window context.
	Awesomium::WebCore*			m_webCore; //!< Pointer to the web engine.
	Awesomium::WebView*			m_webView; //!< Pointer to the web view.
	Awesomium::BitmapSurface*	m_surface; //!< Pointer to the render surface.
};