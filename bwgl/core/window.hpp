#pragma once

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "input.hpp"
#include "debug.hpp"

namespace bwgl {
	enum class FullscreenMode {
		Native = 0,
		Borderless = 1
	};

	struct WindowState {
		// Postion
		int xpos = 0;
		int ypos = 0;

		// Size
		int width = 512;
		int height = 512;
	};

	class Window {
	public:
		static Window& get() {
			static Window instance;
			return instance;
		}

		// By default, fullscreen mode is set to native and vsync is enabled.
		// Does nothing if the window has already been created.
		void create(int width, int height, const char* title, bool resizable = false) {
			if (m_window) {
				BWGL_WARNING(
					"bwgl::Window::create(): window has been created already"
				);

				return;
			}

			// Initialize GLFW
			if (!glfwInit()) {
				BWGL_ERROR(
					"bwgl::Window::create(): failed to initialize GLFW"
				);
			}
			m_glfwInitialized = true;

			// Set OpenGL version 4.4
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Create GLFW window object
			m_window = glfwCreateWindow(
				width,
				height,
				title,
				NULL,
				NULL
			);

			// Create the GLFWwindow object
			if (!m_window) {
				BWGL_ERROR(
					"bwgl::Window::create(): failed to create an OpenGL 4.4 GLFW window"
				);
			}
			glfwMakeContextCurrent(m_window);

			// Setup window settings
			glfwGetWindowPos(m_window, &m_currentState.xpos, &m_currentState.ypos);
			glfwGetWindowSize(m_window, &m_currentState.width, &m_currentState.height);

			m_preFullscreenState.xpos = m_currentState.xpos;
			m_preFullscreenState.ypos = m_currentState.ypos;
			m_preFullscreenState.width = m_currentState.width;
			m_preFullscreenState.height = m_currentState.height;

			// Render settings
			glfwSwapInterval(m_vsync ? 1 : 0);
			setResizable(resizable);

			// Load OpenGL 4.4
			if (!gladLoadGL(glfwGetProcAddress)) {
				BWGL_ERROR(
					"bwgl::Window::create(): failed to initialize GLAD"
				);
			}

			// Enable depth test
			glEnable(GL_DEPTH_TEST);

			// Create an appropriate viewport
			int framebufferWidth;
			int framebufferHeight;

			glfwGetFramebufferSize(
				m_window,
				&framebufferWidth,
				&framebufferHeight
			);

			glViewport(0, 0, framebufferWidth, framebufferHeight);

			Input::get().setWindowDimensions(m_currentState.width, m_currentState.height);
			createCallbacks();

			glfwPollEvents();
		}

		// Polls window events, clears buffers.
		// Does nothing if the window has not been created.
		void beginFrame() {
			if (!m_window) return;
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// Swaps the window buffers, displaying a new frame.
		// Does nothing if the window has not been created.
		void endFrame() {
			if (!m_window) return;
			glfwSwapBuffers(m_window);
		}

		// Sets that the window should close.
		// Does nothing if the window has not been created.
		void close() {
			if (!m_window) return;
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		// Returns true if the window should close or has not been created.
		[[nodiscard]]
		bool shouldClose() const {
			return !m_window || glfwWindowShouldClose(m_window);
		}

		// Overwrites the window color buffer. 
		// Does nothing if the window has not been created.
		void setClearColor(float r, float g, float b, float a = 1.0f) const {
			if (!m_window) return;
			glClearColor(r, g, b, a);
		}

		// Sets fullscreen according to the current fullscreen mode.
		// Does nothing if the window has not been created.
		void setFullscreen(bool value) {
			if (!m_window || value == m_fullscreen) return;

			m_changingFullscreen = true;

			if (m_fullscreenMode == FullscreenMode::Native) {
				setExclusiveFullscreen(value);
			}
			else {
				setBorderlessFullscreen(value);
			}

			m_changingFullscreen = false;
			m_fullscreen = value;
		}

		bool isFullscreen() const {
			return m_fullscreen;
		}

		void setFullscreenMode(FullscreenMode mode) {
			if (mode == m_fullscreenMode) return;

			// If the window is in fullscreen, then that state will have to be re-enabled
			if (m_fullscreen) {
				setFullscreen(false);
				m_fullscreenMode = mode;
				setFullscreen(true);
				
				return;
			}

			m_fullscreenMode = mode;
		}

		FullscreenMode getFullscreenMode() const {
			return m_fullscreenMode;
		}

		// Even non-resizable window can go fullscreen.
		// Does nothing if the window has not been created.
		void setResizable(bool value) {
			if (!m_window) return;
			glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, value);
			m_resizable = value;
		}

		bool isResizable() const {
			return m_resizable;
		}

		// Does nothing if the window has not been created.
		void setVsync(bool value) {
			if (!m_window || value == m_vsync) return;

			glfwSwapInterval(value ? 1 : 0);
			m_vsync = value;
		}

		bool isVsync() const {
			return m_vsync;
		}

		// Does nothing if the window is fullscreen or has not been created.
		void setSize(int width, int height) {
			if (!m_window || m_fullscreen) return;
			glfwSetWindowSize(m_window, width, height);
		}

		// Does nothing if the window is fullscreen or has not been created.
		void setPos(int xpos, int ypos) {
			if (!m_window || m_fullscreen) return;
			glfwSetWindowPos(m_window, xpos, ypos);
		}

		// Does nothing if the window has not been created.
		void setTitle(const std::string& title) {
			if (!m_window) return;
			glfwSetWindowTitle(m_window, title.c_str());
		}

		// Returns an empty string if the window has not been created.
		const char* getTitle() const {
			if (!m_window) return "";
			return glfwGetWindowTitle(m_window);
		}

		// Does nothing if the window has not been created.
		void hideCursor() {
			if (!m_window) return;
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		// Does nothing if the window has not been created.
		void showCursor() {
			if (!m_window) return;
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		// Returns elapsed time since glfw was initialized.
		float getTime() const {
			if (!m_glfwInitialized) return 0.0f;
			return float(glfwGetTime());
		}

		const WindowState& getState() const {
			return m_currentState;
		}

		// Delete copy and move constructors
		Window(const Window&) = delete;
		Window(Window&&) = delete;
	private:
		~Window() {
			if (m_window) {
				glfwDestroyWindow(m_window);
				m_window = nullptr;
			}

			if (m_glfwInitialized) {
				glfwTerminate();
				m_glfwInitialized = false;
			}
		}
		Window() = default;

		// Window settings
		WindowState m_currentState;
		WindowState m_preFullscreenState;

		FullscreenMode m_fullscreenMode = FullscreenMode::Native;
		bool m_changingFullscreen = false;
		
		bool m_fullscreen = false;
		bool m_resizable = false;
		bool m_vsync = true;

		// GLFW handling
		bool m_glfwInitialized = false;
		GLFWwindow* m_window = nullptr;

		// Update window settings.
		void updateSize(int width, int height) {
			m_currentState.width = width;
			m_currentState.height = height;

			if (!m_fullscreen && !m_changingFullscreen) {
				m_preFullscreenState.width = width;
				m_preFullscreenState.height = height;
			}

			// Notify Input
			Input::get().setWindowDimensions(width, height);
		}

		void updateFramebufferSize(int width, int height) {
			glViewport(0, 0, width, height);
		}

		void updatePos(int xpos, int ypos) {
			m_currentState.xpos = xpos;
			m_currentState.ypos = ypos;

			if (!m_fullscreen && !m_changingFullscreen) {
				m_preFullscreenState.xpos = xpos;
				m_preFullscreenState.ypos = ypos;
			}
		}

		void setExclusiveFullscreen(bool value) {
			if (value) {
				// Save windowed state before entering fullscreen
				glfwGetWindowPos(
					m_window,
					&m_preFullscreenState.xpos,
					&m_preFullscreenState.ypos
				);

				glfwGetWindowSize(
					m_window,
					&m_preFullscreenState.width,
					&m_preFullscreenState.height
				);

				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				glfwSetWindowMonitor(
					m_window,
					monitor,
					0,
					0,
					mode->width,
					mode->height,
					mode->refreshRate
				);
			}
			else {
				glfwSetWindowMonitor(
					m_window,
					nullptr,
					m_preFullscreenState.xpos,
					m_preFullscreenState.ypos,
					m_preFullscreenState.width,
					m_preFullscreenState.height,
					0
				);
			}
		}

		void setBorderlessFullscreen(bool value) {
			if (value) {
				// Save windowed state before entering fullscreen
				glfwGetWindowPos(
					m_window,
					&m_preFullscreenState.xpos,
					&m_preFullscreenState.ypos
				);

				glfwGetWindowSize(
					m_window,
					&m_preFullscreenState.width,
					&m_preFullscreenState.height
				);

				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				int monitorX;
				int monitorY;
				glfwGetMonitorPos(monitor, &monitorX, &monitorY);
				
				glfwSetWindowAttrib(
					m_window, 
					GLFW_DECORATED, 
					GLFW_FALSE
				);
				
				glfwSetWindowPos(
					m_window, 
					monitorX, 
					monitorY
				);

				glfwSetWindowSize(
					m_window, 
					mode->width, 
					mode->height
				);
			}
			else {
				glfwSetWindowAttrib(
					m_window, 
					GLFW_DECORATED, 
					GLFW_TRUE
				);

				glfwSetWindowPos(
					m_window, 
					m_preFullscreenState.xpos, 
					m_preFullscreenState.ypos
				);

				glfwSetWindowSize(
					m_window, 
					m_preFullscreenState.width, 
					m_preFullscreenState.height
				);
			}
		}

		void createCallbacks() {
			// Framebuffer size
			glfwSetFramebufferSizeCallback(
				m_window,
				[](GLFWwindow* window, int width, int height) {
					Window::get().updateFramebufferSize(width, height);
				}
			);
			// Window size
			glfwSetWindowSizeCallback(
				m_window,
				[](GLFWwindow* window, int width, int height) {
					Window::get().updateSize(width, height);
				}
			);

			// Window position
			glfwSetWindowPosCallback(
				m_window,
				[](GLFWwindow* window, int xpos, int ypos) {
					Window::get().updatePos(xpos, ypos);
				}
			);

			// Key press
			glfwSetKeyCallback(
				m_window,
				[](GLFWwindow* windowObject, int key, int scancode, int action, int mods) {
					if (action == GLFW_RELEASE) {
						Input::get().setKeyState(key, KeyState::Release);
					}
					else if (action == GLFW_PRESS) {
						Input::get().setKeyState(key, KeyState::Click);
					}
				}
			);

			// Mouse button press
			glfwSetMouseButtonCallback(
				m_window,
				[](GLFWwindow* window, int button, int action, int mods) {
					if (action == GLFW_RELEASE) {
						Input::get().setMouseButtonState(button, KeyState::Release);
					}
					else if (action == GLFW_PRESS) {
						Input::get().setMouseButtonState(button, KeyState::Click);
					}
				}
			);

			// Cursor position
			glfwSetCursorPosCallback(
				m_window,
				[](GLFWwindow* window, double xpos, double ypos) {
					Input::get().setCursorPos(float(xpos), float(ypos));
				}
			);
		}
	};
}