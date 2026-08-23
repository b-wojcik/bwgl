#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "mesh.hpp"
#include "../../core/debug.hpp"

namespace bwgl {
	void loadMesh_ObjFile(Mesh& mesh, const char* filepath) {
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		positions.reserve(128);
		normals.reserve(128);
		uvs.reserve(128);

		std::ifstream input(filepath);
		if (!input.is_open()) {
			BWGL_ERROR(
				"bwgl::loadMesh_ObjFile(): failed to open a file\n",
				"=> ",
				filepath
			);
		}

		std::stringstream buffer;
		buffer << input.rdbuf();

		float x, y, z;
		bool v4_isVertex = false;
		std::string v1, v2, v3, v4 = "";

		int currentIndex = 0;

		std::string word;
		while (!buffer.eof()) {
			if (v4_isVertex) {
				buffer >> word;

				if (word == "v") {
					buffer >> x >> y >> z;
					positions.emplace_back(x, y, z);
				}
				else if (word == "vn") {
					buffer >> x >> y >> z;
					normals.emplace_back(x, y, z);
				}
				else if (word == "vt") {
					buffer >> x >> y;
					uvs.emplace_back(x, y);
				}
				else if (word == "f") {
					buffer >> v1 >> v2 >> v3 >> v4;
					v4_isVertex = v4.find('/') != std::string::npos;

					if (!v4_isVertex) {
					}
				}
			}
			else {
				if (v4 == "v") {
					buffer >> x >> y >> z;
					positions.emplace_back(x, y, z);

					v4_isVertex = true;
					v4 = "";
				}
				else if (v4 == "vn") {
					buffer >> x >> y >> z;
					normals.emplace_back(x, y, z);

					v4_isVertex = true;
					v4 = "";
				}
				else if (v4 == "vt") {
					buffer >> x >> y;
					uvs.emplace_back(x, y);

					v4_isVertex = true;
					v4 = "";
				}
				else if (v4 == "f") {
					buffer >> v1 >> v2 >> v3 >> v4;

					// Make sure that if v4 is v or vn or vt, it gets noted
					if (v4 != "f" && v4.size() <= 2) {
						v4_isVertex = false;
					}
					else {
						v4_isVertex = true;
					}
				}
			}
		}
	}
}