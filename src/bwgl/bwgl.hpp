#pragma once

#include "core/window.hpp"
#include "core/input.hpp"
#include "render/shader_cache.hpp"

namespace bwgl {
	// Singleton class for wrapping up GLFW logic and OpenGL loading, bwgl::Input management.
	inline Window& window = Window::get();

	// Singleton class for handling GLFW keyboard and mouse callbacks.
	inline Input& input = Input::get();

	// Singleton class for loading, storing and using shader programs.
	inline ShaderCache& shader_cache = ShaderCache::get();
}