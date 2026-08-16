#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/debug.hpp"

namespace bwgl {
	typedef GLuint ShaderProgram;

	class ShaderCache {
	public:
		static ShaderCache& get() {
			static ShaderCache instance;
			return instance;
		}

		// Loads and links a shader program from vertex and fragment shaders.
		// Returns its handle on success.
		[[nodiscard]]
		ShaderProgram load(const char* vertexFilePath, const char* fragmentFilePath) {
			bool openFailed = false;

			// Try to open the vertex shader file
			std::ifstream vertexFile(vertexFilePath);
			if (!vertexFile.is_open()) {
				BWGL_WARNING(
					"bwgl::ShaderCache::load(): failed to open vertex shader file:\n",
					"=> ",
					vertexFilePath
				);

				openFailed = true;
			}

			// Try to open the fragment shader file
			std::ifstream fragmentFile(fragmentFilePath);
			if (!fragmentFile.is_open()) {
				BWGL_WARNING(
					"bwgl::ShaderCache::load(): failed to open fragment shader file:\n",
					"=> ",
					fragmentFilePath
				);

				openFailed = true;
			}

			if (openFailed) {
				BWGL_ERROR(
					"bwgl::ShaderCache::load(): failed to load a shader program\n"
				);

				return 0;
			}

			// Used for error checking
			int success;
			char infoLog[512];
			bool compilationFailed = false;

			// Load vertex shader source code
			std::stringstream vertexShaderBuffer;
			vertexShaderBuffer << vertexFile.rdbuf();

			std::string vertexSource = vertexShaderBuffer.str();
			const char* vertexSourcePtr = vertexSource.c_str();

			// Compile the vertex shader
			GLuint vertexShader;
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexSourcePtr, NULL);
			glCompileShader(vertexShader);

			// Check for errors
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

				BWGL_WARNING(
					"bwgl::ShaderCache::load(): vertex shader failed to compile:\n",
					"=> ",
					vertexFilePath,
					'\n',
					infoLog
				);

				compilationFailed = true;
			}

			// Load fragment shader source code
			std::stringstream fragmentShaderBuffer;
			fragmentShaderBuffer << fragmentFile.rdbuf();

			std::string fragmentSource = fragmentShaderBuffer.str();
			const char* fragmentSourcePtr = fragmentSource.c_str();

			// Compile the fragment shader
			GLuint fragmentShader;
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentSourcePtr, NULL);
			glCompileShader(fragmentShader);

			// Check for errors
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

				BWGL_WARNING(
					"bwgl::ShaderCache::load(): fragment shader failed to compile:\n",
					"=> ",
					fragmentFilePath,
					'\n',
					infoLog
				);

				compilationFailed = true;
			}

			if (compilationFailed) {
				BWGL_ERROR(
					"bwgl::ShaderCache::load(): failed to load a shader program\n"
				);

				return 0;
			}

			// Link the shader program
			ShaderProgram program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);

			// Cleanup
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Check for errors
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(program, 512, NULL, infoLog);

				BWGL_ERROR(
					"bwgl::ShaderCache::load(): shader program failed to link:\n",
					"=> ",
					vertexFilePath,
					'\n',
					"=> ",
					fragmentFilePath,
					'\n',
					infoLog
				);

				return 0;
			}

			return program;
		}

		// Tries to use the program.
		void use(ShaderProgram program) {
			if (program == m_lastProgram && m_isProgramValid) {
				return;
			}

			glUseProgram(program);

			m_isProgramValid = true;
			m_lastProgram = program;
		}

		// Invalidates the cached program. 
		// Should be used when glUseProgram() is called externally.
		void invalidate() {
			m_isProgramValid = false;
		}

		// Sets the value of a uniform variable.
		void setBool(ShaderProgram program, const char* name, bool value) {
			glProgramUniform1i(
				program,
				glGetUniformLocation(program, name),
				int(value)
			);
		}

		// Sets the value of a uniform variable.
		void setInt(ShaderProgram program, const char* name, int value) {
			glProgramUniform1i(
				program,
				glGetUniformLocation(program, name),
				value
			);
		}

		// Sets the value of a uniform variable.
		void setFloat(ShaderProgram program, const char* name, float value) {
			glProgramUniform1f(
				program,
				glGetUniformLocation(program, name),
				value
			);
		}

		// Sets the value of a uniform variable.
		void setVec2(ShaderProgram program, const char* name, const GLfloat* ptr) {
			glProgramUniform2fv(
				program,
				glGetUniformLocation(program, name),
				1,
				ptr
			);
		}

		// Sets the value of a uniform variable.
		void setVec2(ShaderProgram program, const char* name, const glm::vec2& vector) {
			glProgramUniform2fv(
				program,
				glGetUniformLocation(program, name),
				1,
				glm::value_ptr(vector)
			);
		}

		// Sets the value of a uniform variable.
		void setVec3(ShaderProgram program, const char* name, const GLfloat* ptr) {
			glProgramUniform3fv(
				program,
				glGetUniformLocation(program, name),
				1,
				ptr
			);
		}

		// Sets the value of a uniform variable.
		void setVec3(ShaderProgram program, const char* name, const glm::vec3& vector) {
			glProgramUniform3fv(
				program,
				glGetUniformLocation(program, name),
				1,
				glm::value_ptr(vector)
			);
		}

		// Sets the value of a uniform variable.
		void setVec4(ShaderProgram program, const char* name, const GLfloat* ptr) {
			glProgramUniform4fv(
				program,
				glGetUniformLocation(program, name),
				1,
				ptr
			);
		}

		// Sets the value of a uniform variable.
		void setVec4(ShaderProgram program, const char* name, const glm::vec4& vector) {
			glProgramUniform4fv(
				program,
				glGetUniformLocation(program, name),
				1,
				glm::value_ptr(vector)
			);
		}

		// Sets the value of a uniform variable.
		void setMat3(ShaderProgram program, const char* name, const GLfloat* ptr) {
			glProgramUniformMatrix3fv(
				program,
				glGetUniformLocation(program, name),
				1,
				GL_FALSE,
				ptr
			);
		}

		// Sets the value of a uniform variable.
		void setMat3(ShaderProgram program, const char* name, const glm::mat3& matrix) {
			glProgramUniformMatrix3fv(
				program,
				glGetUniformLocation(program, name),
				1,
				GL_FALSE,
				glm::value_ptr(matrix)
			);
		}

		// Sets the value of a uniform variable.
		void setMat4(ShaderProgram program, const char* name, const GLfloat* ptr) {
			glProgramUniformMatrix4fv(
				program,
				glGetUniformLocation(program, name),
				1,
				GL_FALSE,
				ptr
			);
		}

		// Sets the value of a uniform variable.
		void setMat4(ShaderProgram program, const char* name, const glm::mat4& matrix) {
			glProgramUniformMatrix4fv(
				program,
				glGetUniformLocation(program, name),
				1,
				GL_FALSE,
				glm::value_ptr(matrix)
			);
		}

		// Delete copy and move constructors
		ShaderCache(const ShaderCache&) = delete;
		ShaderCache(ShaderCache&&) = delete;
	private:
		bool m_isProgramValid = false;
		ShaderProgram m_lastProgram = 0;

		ShaderCache() = default;
		~ShaderCache() = default;
	};
}