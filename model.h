#pragma once

// Std
#include <iostream>
#include <string>
#include <vector>
using namespace std;
// GLEW
#include <GL/glew.h>
// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// Classes
#include "Shader.h"
#include "Mesh.h"

// Functions
GLint textureFromFile(const char* path, string directory);

class Model {
	public:

		// Functions
		Model(GLchar* path)
		{
			this->loadModel(path);
		}
		void draw(Shader shader) {
			for (GLuint i = 0; i < this->meshes.size(); i++) {
				this->meshes[i].Draw(shader);
			}
		}

	private:

		// Model Data
		vector<Mesh> meshes;
		string directory;
		vector<Texture> texturesLoaded;

		// Functions
		void loadModel(string path) {
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
				return;
			}
			this->directory = path.substr(0, path.find_last_of('/'));

			this->processNode(scene->mRootNode, scene);
		}

		void processNode(aiNode* node, const aiScene* scene) {

			// Process all the node's meshes (if any)
			for (GLuint i = 0; i < node->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				this->meshes.push_back(this->processMesh(mesh, scene));
			}

			// Then do the same for each of its children
			for (GLuint i = 0; i < node->mNumChildren; i++) {
				this->processNode(node->mChildren[i], scene);
			}
		}

		Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
			vector<Vertex> vertices;
			vector<GLuint> indices;
			vector<Texture> textures;

			// Vertices
			for (GLuint i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;
				glm::vec3 vector;

				// Position
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.position = vector;

				// Normal
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;

				// Texture coordinates
				if (mesh->mTextureCoords[0]) {
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoords = vec;
				}
				else {
					vertex.texCoords = glm::vec2(0.0f, 0.0f);
				}
				vertices.push_back(vertex);
			}

			// Indices
			for (GLuint i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (GLuint j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			// Material
			if (mesh->mMaterialIndex >= 0) {
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

			return Mesh(vertices, indices, textures);
		}

		vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
			vector<Texture> textures;
			for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
				aiString str;
				mat->GetTexture(type, i, &str);
				GLboolean skip = false;
				for (GLuint j = 0; j < texturesLoaded.size(); j++) {
					if (std::strcmp(texturesLoaded[j].path.C_Str(), str.C_Str()) == 0) {
						textures.push_back(texturesLoaded[j]);
						skip = true;
						break;
					}
				}
				if (!skip) {
					Texture texture;
					texture.id = textureFromFile(str.C_Str(), this->directory);
					texture.type = typeName;
					texture.path = str;
					textures.push_back(texture);
				}
			}
			return textures;
		}
};

GLint textureFromFile(const char* path, string directory) {

	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height, n;
	unsigned char* image = stbi_load(filename.c_str(), &width, &height, &n, 0);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);
	return textureID;
}