/**
* @Author	DJ Coombes
* @date     15th February 2017
* @brief    Shader loader.
*/

#include "shader.h"

#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "logger.h"

namespace ShaderType {
	struct shader_file_extension {
		const char *ext;
		ShaderTypes::ShaderType type;
	};

	struct shader_file_extension extensions[] = {
		{ ".vs", ShaderTypes::VERTEX },
		{ ".vert", ShaderTypes::VERTEX },
		{ ".gs", ShaderTypes::GEOMETRY },
		{ ".geom", ShaderTypes::GEOMETRY },
		{ ".tcs", ShaderTypes::TESS_CONTROL },
		{ ".tes", ShaderTypes::TESS_EVALUATION },
		{ ".fs", ShaderTypes::FRAGMENT },
		{ ".frag", ShaderTypes::FRAGMENT },
		{ ".cs", ShaderTypes::COMPUTE }
	};
}

Shader::Shader() : m_handle(0), m_linked(false) {}

Shader::~Shader() {
	if (m_handle == 0) return;

	// Query number of attached shaders
	GLint numShaders = 0;
	gl::GetProgramiv(m_handle, gl::ATTACHED_SHADERS, &numShaders);

	// Shader names
	GLuint * shaderNames = new GLuint[numShaders];
	gl::GetAttachedShaders(m_handle, numShaders, NULL, shaderNames);

	// Delete shaders
	for (int i = 0; i < numShaders; i++)
		gl::DeleteShader(shaderNames[i]);

	// Delete program
	gl::DeleteProgram(m_handle);

	delete[] shaderNames;
}

void Shader::CompileShader(const char * filename) {
	int numExts = sizeof(ShaderType::extensions) / sizeof(ShaderType::shader_file_extension);

	// Shader type is determined by the file name extension
	std::string ext = getExtension(filename);
	ShaderTypes::ShaderType type = ShaderTypes::VERTEX;
	bool matchFound = false;

	for (int i = 0; i < numExts; i++) {
		if (ext == ShaderType::extensions[i].ext) {
			matchFound = true;
			type = ShaderType::extensions[i].type;
			break;
		}
	}

	if (!matchFound) {
		std::string msg = "Unknown extension: " + ext;
		LOG(DEBUG) << msg;
		return;
	}

	CompileShader(filename, type);
}

std::string Shader::getExtension(const char * name) {
	std::string nameStr(name);

	size_t loc = nameStr.find_last_of('.');
	if (loc != std::string::npos) {
		return nameStr.substr(loc, std::string::npos);
	}

	return "";
}

void Shader::CompileShader(const char * fileName, ShaderTypes::ShaderType type) {
	if (!fileExists(fileName)) {
		std::string message = std::string("Shader: ") + fileName + " not found.";
		LOG(DEBUG) << message;
		return;
	}

	if (m_handle <= 0) {
		m_handle = gl::CreateProgram();
		if (m_handle == 0) {
			LOG(DEBUG) << "Unable to create shader program.";
			return;
		}
	}

	std::ifstream inFile(fileName, std::ios::in);
	if (!inFile) {
		std::string message = std::string("Unable to open: ") + fileName;
		LOG(DEBUG) << message;
		return;
	}

	// File contents
	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();

	CompileShader(code.str(), type, fileName);
}

void Shader::CompileShader(const std::string & source, ShaderTypes::ShaderType type, const char * fileName) {
	if (m_handle <= 0) {
		m_handle = gl::CreateProgram();
		if (m_handle == 0) {
			LOG(DEBUG) << "Unable to create shader program.";
			return;
		}
	}

	GLuint shaderHandle = gl::CreateShader(type);

	const char * c_code = source.c_str();
	gl::ShaderSource(shaderHandle, 1, &c_code, NULL);

	// Compile the shader
	gl::CompileShader(shaderHandle);

	// Check for errors
	int result;
	gl::GetShaderiv(shaderHandle, gl::COMPILE_STATUS, &result);
	if (FALSE == result) {
		// Compile failed, get log
		int length = 0;
		std::string logString;
		gl::GetShaderiv(shaderHandle, gl::INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			gl::GetShaderInfoLog(shaderHandle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}
		std::string msg;
		if (fileName) {
			msg = std::string(fileName) + ": shader compilation failed\n";
		}
		else {
			msg = "Shader compilation failed.\n";
		}
		msg += logString;

		LOG(DEBUG) << msg;
		return;

	}
	else {
		// Compile succeeded, attach shader
		gl::AttachShader(m_handle, shaderHandle);
	}
}

void Shader::Link() {
	if (m_linked) return;
	if (m_handle <= 0) {
		LOG(DEBUG) << "Program has not been compiled.";
		return;
	}
	gl::LinkProgram(m_handle);

	int status = 0;
	gl::GetProgramiv(m_handle, gl::LINK_STATUS, &status);
	if (FALSE == status) {
		// Store log and return false
		int length = 0;
		std::string logString;

		gl::GetProgramiv(m_handle, gl::INFO_LOG_LENGTH, &length);

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			gl::GetProgramInfoLog(m_handle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		LOG(DEBUG) << (std::string("Program link failed:\n") + logString);
		return;
	}
	else {
		m_uniformLocation.clear();
		m_linked = true;
	}
}

void Shader::Use() {
	if (m_handle <= 0 || (!m_linked)) {
		LOG(DEBUG) << "Shader has not been linked";
		return;
	}
	gl::UseProgram(m_handle);
}

int Shader::GetHandle() {
	return m_handle;
}

bool Shader::IsLinked() {
	return m_linked;
}

void Shader::BindAttributeLocation(GLuint location, const char * name) {
	gl::BindAttribLocation(m_handle, location, name);
}

void Shader::BindFragmentDataLocation(GLuint location, const char * name) {
	gl::BindFragDataLocation(m_handle, location, name);
}

void Shader::SetUniform(const char *name, float x, float y, float z) {
	GLint loc = getUniformLocation(name);
	gl::Uniform3f(loc, x, y, z);
}

void Shader::SetUniform(const char *name, const glm::vec2& v) {
	GLint loc = getUniformLocation(name);
	gl::Uniform2f(loc, v.x, v.y);
}
void Shader::SetUniform(const char *name, const glm::vec3& v) {
	this->SetUniform(name, v.x, v.y, v.z);
}

void Shader::SetUniform(const char *name, const glm::vec4& v) {
	GLint loc = getUniformLocation(name);
	gl::Uniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(const char *name, const glm::mat4& m) {
	GLint loc = getUniformLocation(name);
	gl::UniformMatrix4fv(loc, 1, FALSE, &m[0][0]);
}

void Shader::SetUniform(const char *name, const glm::mat3& m) {
	GLint loc = getUniformLocation(name);
	gl::UniformMatrix3fv(loc, 1, FALSE, &m[0][0]);
}

void Shader::SetUniform(const char *name, float val) {
	GLint loc = getUniformLocation(name);
	gl::Uniform1f(loc, val);
}

void Shader::SetUniform(const char *name, int val) {
	GLint loc = getUniformLocation(name);
	gl::Uniform1i(loc, val);
}

void Shader::SetUniform(const char *name, GLuint val) {
	GLint loc = getUniformLocation(name);
	gl::Uniform1ui(loc, val);
}

void Shader::SetUniform(const char *name, bool val) {
	int loc = getUniformLocation(name);
	gl::Uniform1i(loc, val);
}

void Shader::PrintActiveUniforms() {
	GLint numUniforms = 0;
	gl::GetProgramInterfaceiv(m_handle, gl::UNIFORM, gl::ACTIVE_RESOURCES, &numUniforms);

	GLenum properties[] = { gl::NAME_LENGTH, gl::TYPE, gl::LOCATION, gl::BLOCK_INDEX };

	printf("Active uniforms:\n");
	for (int i = 0; i < numUniforms; ++i) {
		GLint results[4];
		gl::GetProgramResourceiv(m_handle, gl::UNIFORM, i, 4, properties, 4, NULL, results);

		if (results[3] != -1) continue;  // Skip uniforms in blocks 
		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		gl::GetProgramResourceName(m_handle, gl::UNIFORM, i, nameBufSize, NULL, name);
		printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
		delete[] name;
	}
}

void Shader::PrintActiveUniformBlocks() {
	GLint numBlocks = 0;

	gl::GetProgramInterfaceiv(m_handle, gl::UNIFORM_BLOCK, gl::ACTIVE_RESOURCES, &numBlocks);
	GLenum blockProps[] = { gl::NUM_ACTIVE_VARIABLES, gl::NAME_LENGTH };
	GLenum blockIndex[] = { gl::ACTIVE_VARIABLES };
	GLenum props[] = { gl::NAME_LENGTH, gl::TYPE, gl::BLOCK_INDEX };

	for (int block = 0; block < numBlocks; ++block) {
		GLint blockInfo[2];
		gl::GetProgramResourceiv(m_handle, gl::UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);
		GLint numUnis = blockInfo[0];

		char * blockName = new char[blockInfo[1] + 1];
		gl::GetProgramResourceName(m_handle, gl::UNIFORM_BLOCK, block, blockInfo[1] + 1, NULL, blockName);
		printf("Uniform block \"%s\":\n", blockName);
		delete[] blockName;

		GLint * unifIndexes = new GLint[numUnis];
		gl::GetProgramResourceiv(m_handle, gl::UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);

		for (int unif = 0; unif < numUnis; ++unif) {
			GLint uniIndex = unifIndexes[unif];
			GLint results[3];
			gl::GetProgramResourceiv(m_handle, gl::UNIFORM, uniIndex, 3, props, 3, NULL, results);

			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			gl::GetProgramResourceName(m_handle, gl::UNIFORM, uniIndex, nameBufSize, NULL, name);
			printf("    %s (%s)\n", name, GetTypeString(results[1]));
			delete[] name;
		}

		delete[] unifIndexes;
	}
}

void Shader::PrintActiveAttributes() {
	GLint numAttribs;
	gl::GetProgramInterfaceiv(m_handle, gl::PROGRAM_INPUT, gl::ACTIVE_RESOURCES, &numAttribs);

	GLenum properties[] = { gl::NAME_LENGTH, gl::TYPE, gl::LOCATION };

	printf("Active attributes:\n");
	for (int i = 0; i < numAttribs; ++i) {
		GLint results[3];
		gl::GetProgramResourceiv(m_handle, gl::PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		gl::GetProgramResourceName(m_handle, gl::PROGRAM_INPUT, i, nameBufSize, NULL, name);
		printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
		delete[] name;
	}
}

const char * Shader::GetTypeString(GLenum type) {
	// There are many more types than are covered here, but
	// these are the most common in these examples.
	switch (type) {
	case gl::FLOAT:
		return "float";
	case gl::FLOAT_VEC2:
		return "vec2";
	case gl::FLOAT_VEC3:
		return "vec3";
	case gl::FLOAT_VEC4:
		return "vec4";
	case gl::DOUBLE:
		return "double";
	case gl::INT:
		return "int";
	case gl::UNSIGNED_INT:
		return "unsigned int";
	case gl::BOOL:
		return "bool";
	case gl::FLOAT_MAT2:
		return "mat2";
	case gl::FLOAT_MAT3:
		return "mat3";
	case gl::FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
}

void Shader::Validate() {
	if (!IsLinked()) {
		LOG(DEBUG) << "Program is not linked";
		return;
	}
	GLint status;
	gl::ValidateProgram(m_handle);
	gl::GetProgramiv(m_handle, gl::VALIDATE_STATUS, &status);

	if (FALSE == status) {
		// Store log and return false
		int length = 0;
		std::string logString;

		gl::GetProgramiv(m_handle, gl::INFO_LOG_LENGTH, &length);

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			gl::GetProgramInfoLog(m_handle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		LOG(DEBUG) << (std::string("Program failed to validate\n") + logString);

	}
}

int Shader::getUniformLocation(const char * name) {
	std::map<std::string, int>::iterator pos;
	pos = m_uniformLocation.find(name);

	if (pos == m_uniformLocation.end()) {
		m_uniformLocation[name] = gl::GetUniformLocation(m_handle, name);
	}

	return m_uniformLocation[name];
}

bool Shader::fileExists(const std::string & fileName) {
	struct stat info;
	int ret = -1;

	ret = stat(fileName.c_str(), &info);
	return 0 == ret;
}