#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <string>

using aie::ShaderProgram;

class Model
{
public:
	Model() = default;
	Model(const std::string& path);
	Model(Mesh mesh);
	~Model();

	std::vector<Mesh> GetMeshes() { return meshes; }

	void Draw(ShaderProgram& shader);
	void LoadMaterials(const std::string& path);
	void ResetModel();

private:
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* mesh, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture> texturesLoaded;
	std::string directory;
};