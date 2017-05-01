#pragma once
#include "Mesh.h"
#include <SOIL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Model {
public:
	std::vector<Mesh> meshes;
	std::string directory;

	Model();
	Model(GLchar*);
	void Draw(Shader, GLint);
	void loadModel(std::string);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);
	GLint TextureFromFile(const char*, std::string);
};