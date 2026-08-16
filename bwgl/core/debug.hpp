#pragma once

#include <iostream>
#include <utility>
#include <cstdlib>

namespace bwgl {
	// !!! Should be called only via the BWGL_ERROR() macro !!!
	template<typename... Args>
	void error(Args&&... args) {
		std::cerr << "BWGL_ERROR: ";
		((std::cerr << std::forward<Args>(args)), ...);
		std::cerr << '\n';

		std::exit(1);
	}

	// !!! Should be called only via the BWGL_WARNING() macro !!!
	template<typename... Args>
	void warning(Args&&... args) {
		std::cerr << "BWGL_WARNING: ";
		((std::cerr << std::forward<Args>(args)), ...);
		std::cerr << '\n';
	}

}

#ifdef BWGL_DEBUG
	// Prints a message, then exits the program.
	#define BWGL_ERROR(...) \
		bwgl::error(__VA_ARGS__)

	// Prints a warning.
	#define BWGL_WARNING(...) \
		bwgl::warning(__VA_ARGS__)
#else
	// Exits the program.
	#define BWGL_ERROR(...) \
		(void)(std::exit(1))

	// Does nothing.
	#define BWGL_WARNING(...) \
		(void)(0)
#endif