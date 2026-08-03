#pragma once

class Shaders {
public:
	static Shaders& get() {
		static Shaders instance;
		return instance;
	}

	// Delete copy and move constructors
	Shaders(const Shaders&) = delete;
	Shaders(Shaders&&) = delete;
private:
	Shaders() = default;
	~Shaders() = default;
};