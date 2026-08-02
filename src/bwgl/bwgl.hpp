#pragma once

#include "core/window.hpp"
#include "core/input.hpp"

namespace bwgl {
	inline Window& window = Window::get();
	inline Input& input = Input::get();
}