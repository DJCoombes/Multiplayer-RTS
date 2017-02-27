/**
* @Author	DJ Coombes
* @date		25th October 2016
* @brief	The user interface class will handle the Awesomium binding, including updating and drawing.
*/

#include "userInterface.h"

#include <chrono>
#include <thread>

#include "logger.h"
#include "glTextureSurface.h"
#include "gl_core_4_4.hpp"

UserInterface::UserInterface(Window& window) : m_window(window) {
	// Create the shaders, link and compile them.
	const char* vertexShaderSource =
		"#version 400\n"
		"layout (location = 0) in vec3 position;"
		"layout (location = 1) in vec3 colour;"
		"layout (location = 2) in vec2 texCoord;"
		"out vec3 outColour;"
		"out vec2 TexCoord;"
		"void main() {"
		"gl_Position = vec4(position, 1.0f);"
		"outColour = colour;"
		"TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);"
		"}";

	const char* fragmentShaderSource =
		"#version 400\n"
		"in vec3 outColour;"
		"in vec2 TexCoord;"
		"out vec4 colour;"
		"uniform sampler2D outTexture;"
		"void main() {"
		"colour = texture(outTexture, TexCoord);"
		"}";

	GLuint vertexShader = gl::CreateShader(gl::VERTEX_SHADER);
	gl::ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	gl::CompileShader(vertexShader);


	GLint success;
	GLchar infoLog[512];
	gl::GetShaderiv(vertexShader, gl::COMPILE_STATUS, &success);
	if (!success) {
		gl::GetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		LOG(ERRORR) << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
	}


	GLuint fragmentShader = gl::CreateShader(gl::FRAGMENT_SHADER);
	gl::ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	gl::CompileShader(fragmentShader);


	gl::GetShaderiv(fragmentShader, gl::COMPILE_STATUS, &success);
	if (!success) {
		gl::GetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		LOG(ERRORR) << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
	}

	m_shaderProgram = gl::CreateProgram();
	gl::AttachShader(m_shaderProgram, vertexShader);
	gl::AttachShader(m_shaderProgram, fragmentShader);
	gl::LinkProgram(m_shaderProgram);

	gl::GetProgramiv(m_shaderProgram, gl::LINK_STATUS, &success);
	if (!success) {
		gl::GetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		LOG(ERRORR) << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
	}

	gl::GenVertexArrays(1, &m_VAO);
	gl::GenBuffers(1, &m_VBO);
	gl::GenBuffers(1, &m_EBO);
}

UserInterface::~UserInterface() {
	m_webView->Destroy();
	m_webCore->Shutdown();
}

bool UserInterface::InitializeUI() {
	// Set the file that's to be used for logging and the logging level.
	Awesomium::WebConfig webConfig;
	webConfig.log_path = Awesomium::WSLit("./UILog.txt");
	webConfig.log_level = Awesomium::kLogLevel_Verbose;
	
	m_webCore = Awesomium::WebCore::Initialize(webConfig);
	if (m_webCore) {
		LOG(INFO) << "Awesomium initialized";
	}
	else {
		LOG(INFO) << "Awesomium was not able to initialize";
		m_window.GetWindow().close();
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Set the type to be used for creating OpenGL surfaces.
	m_webCore->set_surface_factory(new GLTextureSurfaceFactory);

	int width = m_window.GetWidth();
	int height = m_window.GetHeight();

	m_webView = m_webCore->CreateWebView(width, height);
	m_webView->SetTransparent(true);
	// Create a global object from which JavaScript functions can be called from C++.
	m_result = m_webView->CreateGlobalJavascriptObject(Awesomium::WSLit("Engine"));



	m_engineObject = m_result.ToObject();
	m_webView->set_js_method_handler(&m_methodDispatcher);
	// Load the custom web page that's to be used for the user interface.
	Awesomium::WebURL url(Awesomium::WSLit("file:///./resources/webui/index.html"));
	m_webView->LoadURL(url);
	// Bind my logging function.
	BindMethod(Awesomium::WSLit("Log"), &UserInterface::WebLog, this);

	m_webWindow = m_webView->ExecuteJavascriptWithResult(Awesomium::WSLit("window"), Awesomium::WSLit(""));

	LOG(DEBUG) << "Custom HTML file loaded.";
	LOG(INFO) << "User interface successfully initialized.";

	return true;
}

void UserInterface::HandleEvent(sf::Event& event) {
	if (m_webView) {
		m_inputForwarder.HandleEvent(m_webView, event);
	}
}

void UserInterface::UpdateView() {
	while (m_webView->IsLoading())
		m_webCore->Update();

	m_webCore->Update();
}

void UserInterface::DrawUI() {
	if (m_webView == NULL)
		return;
	// Create the OpenGL surface to render.
	const Awesomium::Surface* webSurface = m_webView->surface();
	const GLTextureSurface* textureSurface = static_cast<const GLTextureSurface*>(webSurface);

	if (textureSurface == nullptr)
		return;
	// Set the texture to transparent,
	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::ONE, gl::ONE_MINUS_SRC_ALPHA);

	gl::UseProgram(m_shaderProgram);
	gl::BindTexture(gl::TEXTURE_2D, textureSurface->GetTexture());

	gl::BindVertexArray(m_VAO);
	
	GLfloat vertices[] = {
		// Positions          // Colours           // Texture Coordinates
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	// Bind the GL states.
	gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), vertices, gl::STATIC_DRAW);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, m_EBO);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, gl::STATIC_DRAW);


	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	gl::EnableVertexAttribArray(0);

	gl::VertexAttribPointer(1, 3, gl::FLOAT, FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(1);

	gl::VertexAttribPointer(2, 2, gl::FLOAT, FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	gl::EnableVertexAttribArray(2);
	// Draw the UI.
	gl::DrawElements(gl::TRIANGLES, 6, gl::UNSIGNED_INT, 0);
	// Unbind all the GL states.
	gl::DisableVertexAttribArray(0);
	gl::DisableVertexAttribArray(1);
	gl::DisableVertexAttribArray(2);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);
	gl::UseProgram(0);
	gl::BindVertexArray(0);
}

void UserInterface::Resize() {
	int width = m_window.GetWidth();
	int height = m_window.GetHeight();

	m_webView->Resize(width, height);
}

Awesomium::WebView* UserInterface::GetWebView() {
	return m_webView;
}

void UserInterface::CallJSFunc(Awesomium::WebString& funcName, Awesomium::JSArray args) {
	if (m_webWindow.IsObject()) {
		m_webWindow.ToObject().Invoke(funcName, args);
	}
}

void UserInterface::UnBindMethod(const std::string& name) {
	Awesomium::WebString tempName = Awesomium::WSLit(name.c_str());
	m_methodDispatcher.Delete(m_engineObject, tempName);
}

void UserInterface::WebLog(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
	Awesomium::JSValue logInfo = args.At(0);
	LOG(INFO) << logInfo.ToString();
}