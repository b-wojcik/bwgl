#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"

#include "../core/debug.hpp"

namespace bwgl {
	typedef GLuint Texture;

	class TextureCache {
	public:
		static TextureCache& get() {
			static TextureCache instance;
			return instance;
		}

		// Loads the texture from a file, returns its handle on success.
		[[nodiscard]]
		Texture load(const char* filepath) {
			Texture texture;

			// Create the texture
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

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
				BWGL_WARNING(
					"bwgl::TextureCache::load(): failed to load image data from:\n",
					"=> ",
					filepath
				);
			}
			stbi_image_free(data);

			return texture;
		}

		// Tries to use the texture.
		void use(Texture texture) {
			if (texture == m_lastTexture && m_isTextureValid) {
				return;
			}

			glBindTexture(GL_TEXTURE_2D, texture);

			// Save the new state
			m_isTextureValid = true;
			m_lastTexture = texture;
		}

		// Invalidates the cached texture. 
		// Should be used when glBindTexture() is called externally.
		void invalidate() {
			m_isTextureValid = false;
		}

		// Delete copy and move constructors
		TextureCache(const TextureCache&) = delete;
		TextureCache(TextureCache&&) = delete;
	private:
		bool m_isTextureValid = false;
		Texture m_lastTexture = 0;

		TextureCache() = default;
		~TextureCache() = default;
	};
}