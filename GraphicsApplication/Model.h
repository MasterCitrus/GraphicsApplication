#pragma once
#include "Shader.h"
#include "Animation.h"
#include "Animator.h"
#include "Mesh.h"
#include "BoneInfo.h"
#include <vector>
#include <map>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <string>

using aie::ShaderProgram;

class Model
{
public:
	Model() = default;
	Model(const char* path);
	Model(Mesh* mesh);
	~Model();

	std::vector<Mesh*> GetMeshes() { return meshes; }
	std::vector<Animation*> GetAnimations() { return animations; }
	Animator* GetAnimator() { return animator; }
	std::string& GetName() { return name; }
	std::map<std::string, BoneInfo>& GetBoneInfoMap() { return boneInfoMap; }
	int& GetBoneCount() { return boneCounter; }

	void Update(float delta);
	void Draw(ShaderProgram* shader);
	void LoadModel(const char* path);
	void LoadMaterials(const char* path);
	void LoadAnimations(const aiScene* scene);
	void ResetModel();

private:
	void ProcessNode(aiNode* mesh, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Animation* ProcessAnimation(aiAnimation* animation, aiNode* node);

	void SetVertexToBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

private:
	std::vector<Mesh*> meshes;
	std::vector<Animation*> animations;
	std::map<std::string, BoneInfo> boneInfoMap;
	std::string name;
	Animator* animator;
	int boneCounter = 0;
};