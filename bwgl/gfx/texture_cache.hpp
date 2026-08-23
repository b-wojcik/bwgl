#pragma once

#include <array>

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

		// Tries to use the texture for specified texture unit [0;31].
		void use(Texture texture, unsigned int unit) {
			if (unit > 31) {
				BWGL_ERROR(
					"bwgl::TextureCache::use(): unit out of bounds [0;31]:\n",
					"=> ",
					unit
				);
				return;
			}

			// Avoid unnecessary OpenGL state changes
			if (texture == m_lastTexture[unit] && m_isTextureValid[unit]) {
				return;
			}

			// GL_TEXTURE0<->GL_TEXTURE31 range is continous
			glActiveTexture(GL_TEXTURE0 + unit);

			glBindTexture(GL_TEXTURE_2D, texture);

			// Save the new state
			m_lastTexture[unit] = texture;
			m_isTextureValid[unit] = true;
		}

		// Invalidates the cached texture for specified texture unit [0;31]. 
		// Should be used when glBindTexture() is called externally for texture unit.
		void invalidate(unsigned int unit) {
			if (unit > 31) {
				BWGL_ERROR(
					"bwgl::TextureCache::invalidate(): unit out of bounds [0;31]:\n",
					"=> ",
					unit
				);
				return;
			}
			
			m_isTextureValid[unit] = false;
		}

		// Delete copy and move constructors
		TextureCache(const TextureCache&) = delete;
		TextureCache(TextureCache&&) = delete;
	private:
		std::array<bool, 32> m_isTextureValid;
		std::array<Texture, 32> m_lastTexture;

		TextureCache() {
			m_isTextureValid.fill(false);
			m_lastTexture.fill(0);
		}

		~TextureCache() = default;
	};
}