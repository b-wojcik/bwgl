#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glad/gl.h>

#include "vertex.hpp"
#include "../../core/debug.hpp"

namespace bwgl {
	class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		// Uploads mesh data to the GPU.
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

		// This function supports only .obj files!
		// Loads mesh vertices and indices member vectors.
		void loadFromFile(const std::string& filepath) {
			if (filepath.size() < 5) {
				BWGL_ERROR(
					"bwgl::Mesh::loadFromFile(): the filepath is too short:\n",
					"=> ",
					filepath
				);
			}

			int last = filepath.size() - 1;
			if (
				filepath[last - 3] != '.' ||
				filepath[last - 2] != 'o' ||
				filepath[last - 1] != 'b' ||
				filepath[last] != 'j'
				) {
				BWGL_ERROR(
					"bwgl::Mesh::loadFromFile(): only .obj files are supported:\n",
					"=> ",
					filepath
				);
			}

			std::vector<glm::vec3> positions;
			positions.reserve(128);

			std::vector<glm::vec3> normals;
			normals.reserve(128);

			std::vector<glm::vec2> uvs;
			uvs.reserve(128);

			auto loadVertex = [&](const std::string& str) {
				int state = 0;
				int len = 0;

				int lastSlashIndex = 0;

				int pos = 1;
				int uv = 1;
				int norm = 1;

				for (int i = 0; i < str.size(); i++) {
					if (str[i] != '/') {
						len++;
						continue;
					}

					// Get all characters in between slashes
					int value = std::stoi(str.substr(i - len, len));

					if (state) {
						uv = value;
						lastSlashIndex = i;

						break;
					}

					// For the state 0
					pos = value;

					// Reset for the next value
					len = 0;
					state++;
				}

				norm = std::stoi(str.substr(lastSlashIndex + 1));

				return Vertex {
					positions[pos - 1],
					normals[norm - 1],
					uvs[uv - 1]
				};
			};

			std::ifstream file(filepath);
			if (!file.is_open()) {
				BWGL_ERROR(
					"bwgl::Mesh::loadFromFile(): failed to open a file\n",
					"=> ",
					filepath
				);
			}

			std::string line;
			std::stringstream buffer;
			std::string entry;

			float x, y, z;

			while (std::getline(file, line)) {
				buffer.clear();
				buffer.str(line);

				buffer >> entry;
				if (entry == "v") {
					// x
					buffer >> x;
					// y
					buffer >> y;
					// z
					buffer >> z;

					// Omit the w coordinate
					positions.emplace_back(x, y, z);
				}
				else if (entry == "vn") {
					// i
					buffer >> x;
					// j
					buffer >> y;
					// k
					buffer >> z;

					normals.emplace_back(x, y, z);
				}
				else if (entry == "vt") {
					// u
					buffer >> x;
					// v
					if (!(buffer >> y)) {
						y = 0.0f;
					}

					// Omit the w coordinate
					uvs.emplace_back(x, y);
				}
				else if (entry == "f") {
					int vertexCount = 0;

					// Load the face which is an n-polygon
					while (buffer >> entry) {
						vertices.emplace_back(loadVertex(entry));
						vertexCount++;
					}

					// This algorithm doesn't make sure that each vertex is stored only once in memory...
					// Although some models may reuse vertices so that back face culling does not affect some faces
					int globalOffset = 0;
					if (indices.size() > 0) {
						// Last value of indices is always the current biggest index value
						globalOffset = indices[indices.size() - 1] + 1;
					}

					int offset = 1;
					for (int i = 0; i < vertexCount - 2; i++) {
						// The index 0 is always stationary
						indices.push_back(globalOffset + 0);
						indices.push_back(globalOffset + offset);
						indices.push_back(globalOffset + offset + 1);

						offset++;
					}
				}
			}
			file.close();
		}
	private:
		bool created = false;
		bool warningShown = false;

		GLuint VAO = 0;
	};
}