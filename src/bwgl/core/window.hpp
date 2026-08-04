#pragma once

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "input.hpp"

namespace bwgl {
	enum class FullscreenMode {
		Native = 0,
		Borderless = 1
	};

	// Stores windows position and size.
	struct WindowState {
		// Postion
		int xpos = 0;
		int ypos = 0;

		// Size
		int width = 512;
		int height = 512;
	};

	// Singleton class for wrapping up GLFW logic and OpenGL loading, bwgl::Input management.
	class Window {
	public:
		static Window& get() {
			static Window instance;
			return instance;
		}

		// Initializes all members of Window, loads OpenGL and sets up Input.
		// This function can be called only once, for Window is meant as a singleton.
		[[nodiscard]]
		bool create(int width, int height, const char* title, bool vsync = false) {
			if (m_window) return false;

			if (!glfwInit()) {
				std::cerr << "ERROR bwgl::Window::create() | Failed to initialize GLFW\n";
				return false;
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

			if (!m_window) {
				std::cerr << "ERROR bwgl::Window::create() | Failed to create an OpenGL 4.4 GLFW window\n";
				
				glfwTerminate();
				m_glfwInitialized = false;

				return false;
			}

			glfwMakeContextCurrent(m_window);

			// Setup window settings
			glfwGetWindowPos(m_window, &m_currentState.xpos, &m_currentState.ypos);
			glfwGetWindowSize(m_window, &m_currentState.width, &m_currentState.height);

			m_preFullscreenState.xpos = m_currentState.xpos;
			m_preFullscreenState.ypos = m_currentState.ypos;
			m_preFullscreenState.width = m_currentState.width;
			m_preFullscreenState.height = m_currentState.height;

			m_fullscreen = false;
			m_vsync = vsync;
			glfwSwapInterval(vsync ? 1 : 0);

			// Load OpenGL 4.4
			if (!gladLoadGL(glfwGetProcAddress)) {
				std::cerr << "ERROR bwgl::Window::create() | Failed to initialize GLAD\n";
				
				glfwDestroyWindow(m_window);
				m_window = nullptr;

				glfwTerminate();
				return false;
			}

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

			return true;
		}

		// Polls window events.
		void pollEvents() {
			if (!m_window) return;
			glfwPollEvents();
		}

		// Swaps windows buffers.
		void present() {
			if (!m_window) return;
			glfwSwapBuffers(m_window);
		}

		// Sets that the window should close.
		void close() {
			if (!m_window) return;
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		// Returns true if the window is said to close, or the GLFWwindow object has not been created.
		[[nodiscard]]
		bool shouldClose() const {
			if (!m_window) return true;
			return glfwWindowShouldClose(m_window);
		}

		// Sets fullscreen according to the current FullscreenMode.
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

		// The FullscreenMode can only be updated when the window is in windowed mode.
		// Returns true when the FullscreenMode changes successfully.  
		bool setFullscreenMode(FullscreenMode mode) {
			if (m_fullscreen || m_changingFullscreen) {
				return false;
			}

			m_fullscreenMode = mode;

			return true;
		}

		FullscreenMode getFullscreenMode() const {
			return m_fullscreenMode;
		}

		// Even when the resizable setting is set to false, the window can still go fullscreen.
		// Fails when the GLFWwindow object has not been created.
		void setResizable(bool value) {
			if (!m_window) return;
			glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, value);
			m_resizable = value;
		}

		bool isResizable() const {
			return m_resizable;
		}

		// Changes the vsync settings.
		// Fails when the GLFWwindow object has not been created.
		void setVsync(bool value) {
			if (!m_window || value == m_vsync) return;

			glfwSwapInterval(value ? 1 : 0);
			m_vsync = value;
		}

		bool isVsync() const {
			return m_vsync;
		}

		// Changes the window state.
		// Fails when window is in fullscreen, or the GLFWwindow object has not been created.
		void setSize(int width, int height) {
			if (!m_window || m_fullscreen) return;
			glfwSetWindowSize(m_window, width, height);
		}

		// Changes the window state.
		// Fails when window is in fullscreen, or the GLFWwindow object has not been created.
		void setPos(int xpos, int ypos) {
			if (!m_window || m_fullscreen) return;
			glfwSetWindowPos(m_window, xpos, ypos);
		}

		WindowState getState() const {
			return m_currentState;
		}

		// Copy and move constructors
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
		bool m_resizable = true;
		
		bool m_vsync = false;

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