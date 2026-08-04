#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

// Singleton class for loading, storing and using shader programs.
class ShaderCache {
public:
	static ShaderCache& get() {
		static ShaderCache instance;
		return instance;
	}

	// Links a shader program for specified vertex and fragment shaders.
	// From then on, that shader program can be used.
	void load(const char* name, const char* vertexFilePath, const char* fragmentFilePath) {
		bool openFailed = false;

		// Try to open the vertex shader file
		std::ifstream vertexFile(vertexFilePath);
		if (!vertexFile.is_open()) {
			std::cerr << "ERROR bwgl::ShaderCache::load() | Failed to open vertex shader file:\n";
			std::cerr << "	" << '"' << vertexFilePath << '"' << '\n';
			openFailed = true;
		}

		// Try to open the fragment shader file
		std::ifstream fragmentFile(fragmentFilePath);
		if (!fragmentFile.is_open()) {
			std::cerr << "ERROR bwgl::ShaderCache::load() | Failed to open fragment shader file:\n";
			std::cerr << "	" << '"' << fragmentFilePath << '"' << '\n';
			openFailed = true;
		}

		if (openFailed) {
			return;
		}

		// Load vertex shader source code
		std::stringstream vertexShaderBuffer;
		vertexShaderBuffer << vertexFile.rdbuf();
		
		std::string vertexSource = vertexShaderBuffer.str();
		const char* vertexSourcePtr = vertexSource.c_str();

		// Load fragment shader source code
		std::stringstream fragmentShaderBuffer;
		fragmentShaderBuffer << fragmentFile.rdbuf();

		std::string fragmentSource = fragmentShaderBuffer.str();
		const char* fragmentSourcePtr = fragmentSource.c_str();

		// Used for error checking
		int success;
		char infoLog[512];
		bool compilationFailed = false;

		// Compile the vertex shader
		GLuint vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSourcePtr, NULL);
		glCompileShader(vertexShader);

		// Check for errors
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

			std::cerr << "ERROR bwgl::ShaderCache::load() | Vertex shader failed to compile:\n";
			std::cerr << vertexFilePath << '\n';
			std::cerr << infoLog << '\n';
			compilationFailed = true;
		}

		// Compile the fragment shader
		GLuint fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSourcePtr, NULL);
		glCompileShader(fragmentShader);

		// Check for errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

			std::cerr << "ERROR bwgl::ShaderCache::load() | Fragment shader failed to compile:\n";
			std::cerr << fragmentFilePath << '\n';
			std::cerr << infoLog << '\n';
			compilationFailed = true;
		}

		if (compilationFailed) {
			return;
		}

		// Link the shader program
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		// Check for errors
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);

			std::cerr << "ERROR bwgl::ShaderCache::load() | Shader program failed to link:\n";
			std::cerr << infoLog << '\n';
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Add the shader program
		shaders[name] = programID;
	}

	// Tries to use a specified shader program, prints a warning if fails.
	void use(const char* name) {
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::use() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}

		glUseProgram(shaders[name]);
	}

	// Makes OpenGL use the default shader program.
	inline void disuse() {
		glUseProgram(0);
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setBool(const char* name, const char* variableName, bool value) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setBool() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniform1i(programID, glGetUniformLocation(programID, variableName), int(value));
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setInt(const char* name, const char* variableName, int value) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setInt() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniform1i(programID, glGetUniformLocation(programID, variableName), value);
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setFloat(const char* name, const char* variableName, float value) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setFloat() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniform1f(programID, glGetUniformLocation(programID, variableName), value);
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setVec2(const char* name, const char* variableName, const GLfloat* ptr) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setVec2() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniform2fv(programID, glGetUniformLocation(programID, variableName), 1, ptr);
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setVec3(const char* name, const char* variableName, const GLfloat* ptr) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setVec3() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniform3fv(programID, glGetUniformLocation(programID, variableName), 1, ptr);
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setVec4(const char* name, const char* variableName, const GLfloat* ptr) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setVec4() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniform4fv(programID, glGetUniformLocation(programID, variableName), 1, ptr);
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setMat3(const char* name, const char* variableName, const GLfloat* ptr) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setMat3() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniformMatrix3fv(
			programID,
			glGetUniformLocation(programID, variableName),
			1,
			GL_FALSE,
			ptr
		);
	}

	// Sets the value of a uniform variable for a specified shader program.
	// If the shader program is not found, prints a warning.
	void setMat4(const char* name, const char* variableName, const GLfloat* ptr) {
		// Try to find the shader program id
		if (!shaders.count(name)) {
			std::cerr << "WARNING bwgl::ShaderCache::setMat4() | Shader not found:\n";
			std::cerr << name << '\n';
			return;
		}
		GLuint programID = shaders[name];

		// Upload the data
		glProgramUniformMatrix4fv(
			programID, 
			glGetUniformLocation(programID, variableName), 
			1, 
			GL_FALSE,
			ptr
		);
	}
		 
	// Delete copy and move constructors
	ShaderCache(const ShaderCache&) = delete;
	ShaderCache(ShaderCache&&) = delete;
private:
	// Maps shader names to their OpenGL IDs 
	std::unordered_map<std::string, GLuint> shaders;

	ShaderCache() = default;
	~ShaderCache() = default;
};