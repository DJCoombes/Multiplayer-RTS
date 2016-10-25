/**
* @Author	DJ Coombes
* @date		25th October 2016
* @brief	The user interface class will handle the Awesomium binding, including updating and drawing.
*/

#include "userInterface.h"

#include "logger.h"
#include "glTextureSurface.h"

UserInterface::UserInterface(Window& window) : m_window(window) {

}

UserInterface::~UserInterface() {
	m_webView->Destroy();
	m_webCore->Shutdown();
}

bool UserInterface::InitializeUI() {
	Awesomium::WebConfig webConfig;
	webConfig.log_path = Awesomium::WSLit("./UILog.txt");
	webConfig.log_level = Awesomium::kLogLevel_Verbose;

	m_webCore = Awesomium::WebCore::Initialize(webConfig);
	if (m_webCore) {
		LOG(INFO) << "Awesomium initialized";
	}
	else {
		return false;
	}
	m_webCore->set_surface_factory(new GLTextureSurfaceFactory);

	int width = m_window.GetWidth();
	int height = m_window.GetHeight();

	m_webView = m_webCore->CreateWebView(width, height);
	m_webView->SetTransparent(true);

	Awesomium::WebURL url(Awesomium::WSLit("http://www.google.com"));
	m_webView->LoadURL(url);
	LOG(DEBUG) << "Custom HTML file loaded.";
	LOG(INFO) << "User interface successfully initialized.";

	return true;
}

void UserInterface::UpdateView() {
	while (m_webView->IsLoading())
		m_webCore->Update();

	m_webCore->Update();
}

void UserInterface::DrawUI() {
	if (m_webView == NULL)
		return;

	const Awesomium::Surface* webSurface = m_webView->surface();
	const GLTextureSurface* textureSurface = static_cast<const GLTextureSurface*>(webSurface);

	int tileWidth = textureSurface->width();
	int tileHeight = textureSurface->height();

	GLfloat Vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	GLfloat TexCoord[] = { 
		0, 1,
		1, 1,
		0, 0,
		1, 0,
	};

	glViewport(0, 0, tileWidth, tileHeight);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glTexCoordPointer(2, GL_SHORT, 0, TexCoord);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEnable(GL_TEXTURE_2D);

#if TRANSPARENT
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif

	glBindTexture(GL_TEXTURE_2D, textureSurface->GetTexture());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void UserInterface::Resize() {
	int width = m_window.GetWidth();
	int height = m_window.GetHeight();

	m_webView->Resize(width, height);
}

Awesomium::WebView* UserInterface::GetWebView() {
	return m_webView;
}