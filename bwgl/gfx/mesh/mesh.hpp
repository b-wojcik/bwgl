#pragma once

#include <vector>
#include <glad/gl.h>

#include "vertex.hpp"

#include "../../core/debug.hpp"

namespace bwgl {
	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		// Creates mesh using vertices and indices member vectors.
		// Does nothing if the mesh has already been created.
		void create() {
			if (created) {
				BWGL_WARNING("bwgl::Mesh::create(): mesh already created");
				return;
			}
			created = true;

			// Generate OpenGL buffers
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			// Load vertices
			glBufferData(
				GL_ARRAY_BUFFER,
				vertices.size() * sizeof(Vertex),
				&vertices[0],
				GL_STATIC_DRAW
			);

			// Enable vertex position
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vertex),
				(void*)0
			);
			glEnableVertexAttribArray(0);

			// Enable vertex normal
			glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vertex),
				(void*)offsetof(Vertex, normal)
			);
			glEnableVertexAttribArray(1);

			// Enable vertex texture UV
			glVertexAttribPointer(
				2,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(Vertex),
				(void*)offsetof(Vertex, textureUV)
			);
			glEnableVertexAttribArray(2);

			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			// Load indices
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER, 
				indices.size() * sizeof(GLuint),
				&indices[0], 
				GL_STATIC_DRAW
			);

			// Cleanup
			glBindVertexArray(0);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		// Creates mesh using vertices and indices function parameters.
		// Does nothing if the mesh has already been created.
		void create(
			const std::vector<float>& vertices,
			const std::vector<unsigned int>& indices) {

			if (created) {
				BWGL_WARNING("bwgl::Mesh::create(): mesh already created");
				return;
			}

			// Load Mesh::vertices
			const int vertexCount = vertices.size() / 8;
			Mesh::vertices.reserve(vertexCount);
			Vertex vertex;

			const int stride = 8;
			for (int i = 0; i < vertexCount; i++) {
				int offset = i * stride;

				// Position
				vertex.position.x = vertices[offset];
				vertex.position.y = vertices[1 + offset];
				vertex.position.z = vertices[2 + offset];

				// Normal
				vertex.normal.x = vertices[3 + offset];
				vertex.normal.y = vertices[4 + offset];
				vertex.normal.z = vertices[5 + offset];

				// Texture UV
				vertex.textureUV.x = vertices[6 + offset];
				vertex.textureUV.y = vertices[7 + offset];

				Mesh::vertices.emplace_back(vertex);
			}

			Mesh::indices = indices;

			create();
		}

		// Does nothing if the mesh has not been created.
		void draw() {
			if (!created) {
				if (!warningShown) {
					BWGL_WARNING("bwgl::Mesh::draw(): mesh not created");
					warningShown = true;
				}
				return;
			}

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
	private:
		bool created = false;
		bool warningShown = false;

		GLuint VAO = 0;
	};
}