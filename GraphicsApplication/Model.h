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
	Model(const char* path);
	~Model();

	std::vector<Mesh*> GetMeshes() { return meshes; }

	void Draw(ShaderProgram* shader);
	void LoadModel(const char* path);
	void LoadMaterials(const char* path);
private:
	std::vector<Mesh*> meshes;

	void ProcessNode(aiNode* mesh, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
};