#pragma once

#include <glad/gl.h>

namespace bwgl {
	class Plane {
	public:
		Plane() {
			static const float vertices[20] = {
				// X     Y     Z     U     V
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
			};

			static const unsigned int indices[6] = {
				0, 1, 2,
				2, 3, 0
			};

			// Create the vertex array object
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			// Generate and bind a vertex buffer object
			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			// Upload buffer data and enable vertex attribute pointers
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);

			// Generate and bind an element buffer object
			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			// Upload buffer data
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// Cleanup
			glBindVertexArray(0);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		void draw() {
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	private:
		GLuint VAO;
	};
}