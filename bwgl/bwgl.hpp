#pragma once

#include "core/window.hpp"
#include "gfx/shader_cache.hpp"
#include "gfx/texture_cache.hpp"

namespace bwgl {
	inline Window& window = Window::get();
	
	inline Input& input = Input::get();
	
	inline ShaderCache& shader_cache = ShaderCache::get();

	inline TextureCache& texture_cache = TextureCache::get();

	// Returns the elapsed time since the window was created.
	inline float time() {
		return window.getTime();
	}
}