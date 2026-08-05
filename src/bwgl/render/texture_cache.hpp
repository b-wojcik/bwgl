#pragma once

#include <string>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Singleton class for loading, storing and using 2D textures.
// Warning: Manually calling glBindTexture(...) will likely break the TextureCache.use() function! 
// For optimization reasons, the last used texture is remembered, as to minimize OpenGL state changes.
class TextureCache {
public:
	static TextureCache& get() {
		static TextureCache instance;
		return instance;
	}

	// Loads a texture from a file, it can later be acessed by the assigned name.
	void load(const char* name, const char* filepath) {
		GLuint textureID;

		// Create the texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Setup wrapping and filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load texture data
		stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

		// Check if data was loaded correctly
		if (data) {
			// Make sure that the texture color data is interpreted correctly
			GLenum format = GL_RGB;
			if (nrChannels == 1) {
				format = GL_RED;
			}
			else if (nrChannels == 3) {
				format = GL_RGB;
			}
			else if (nrChannels == 4) {
				format = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cerr << "ERROR Texture::load() | Failed to load image data for:\n";
			std::cerr << '"' << filepath << '"' << '\n';
		}
		stbi_image_free(data);

		// Upload the generated texture
		textures[name] = textureID;
		disuse();
	}

	// Tries to use a specified texture, prints a warning if fails.
	void use(const char* name) {
		if (!textures.count(name)) {
			std::cerr << "WARNING bwgl::TextureCache::use() | Texture not found:\n";
			std::cerr << '"' << name << '"' << '\n';
			return;
		}

		// Don't change OpenGL state, if it is not necessary
		GLuint newTexture = textures[name];
		if (newTexture == lastUsedTexture) {
			return;
		}

		lastUsedTexture = newTexture;
		glBindTexture(GL_TEXTURE_2D, newTexture);
	}

	// Makes OpenGL use no textures.
	inline void disuse() {
		glBindTexture(GL_TEXTURE_2D, 0);
		lastUsedTexture = 0;
	}

	// Delete copy and move constructors
	TextureCache(const TextureCache&) = delete;
	TextureCache(TextureCache&&) = delete;
private:
	// Maps texture names to their OpenGL IDs 
	std::unordered_map<std::string, GLuint> textures;

	GLuint lastUsedTexture = 0;

	TextureCache() = default;
	~TextureCache() = default;
};