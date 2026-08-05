#pragma once

#include "core/window.hpp"
#include "core/input.hpp"
#include "render/shader_cache.hpp"
#include "render/texture_cache.hpp"
#include "render/test_mesh.hpp"

namespace bwgl {
	// Singleton class for wrapping up GLFW logic and OpenGL loading, bwgl::Input management.
	inline Window& window = Window::get();

	// Singleton class for handling GLFW keyboard and mouse callbacks.
	inline Input& input = Input::get();

	// Singleton class for loading, storing and using shader programs.
	// Warning: Manually calling glUseProgram(...) will likely break the ShaderCache.use() function! 
	// For optimization reasons, the last used shader program is remembered, as to minimize OpenGL state changes.
	inline ShaderCache& shader_cache = ShaderCache::get();

	// Singleton class for loading, storing and using 2D textures.
	// Warning: Manually calling glBindTexture(...) will likely break the TextureCache.use() function! 
	// For optimization reasons, the last used texture is remembered, as to minimize OpenGL state changes.
	inline TextureCache& texture_cache = TextureCache::get();
}