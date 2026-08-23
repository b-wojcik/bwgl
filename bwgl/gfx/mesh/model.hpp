#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"

namespace bwgl {
	class Model {
	public:
		void create(const char* filepath) {
			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(
				filepath, 
				aiProcess_Triangulate
			);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				BWGL_ERROR(
					"bwgl::Model::create(): failed to create model:\n",
					"=> ",
					filepath
				);

				return;
			}

			processNode(scene->mRootNode, scene);
			for (auto& mesh : meshes) {
				mesh.create();
			}
		}

		void draw() {
			for (auto& mesh : meshes) {
				mesh.draw();
			}
		}
	private:
		std::vector<Mesh> meshes;

		Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
			Mesh result;
			result.vertices.reserve(mesh->mNumVertices);

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;

				// Positions
				vertex.position.x = mesh->mVertices[i].x;
				vertex.position.y = mesh->mVertices[i].y;
				vertex.position.z = mesh->mVertices[i].z;

				// Normal
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;

				// UV
				if (mesh->mTextureCoords[0]) {
					vertex.textureUV.x = mesh->mTextureCoords[0][i].x;
					vertex.textureUV.y = mesh->mTextureCoords[0][i].y;
				}
				else {
					vertex.textureUV = glm::vec2(0.0f);
				}

				result.vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++) {
					result.indices.push_back(face.mIndices[j]);
				}
			}

			return result;
		}

		void processNode(aiNode* node, const aiScene* scene) {
			for (unsigned int i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++) {
				processNode(node->mChildren[i], scene);
			}
		}
	};
}