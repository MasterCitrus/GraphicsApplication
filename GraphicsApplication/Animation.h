#pragma once
#include "BoneInfo.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <map>

class Bone;
class Model;

struct aiAnimation;
struct aiNode;
struct aiScene;

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;
	Animation(aiNode* node, aiAnimation* animation, Model* model);
	Animation(const std::string& animationPath, Model* model);
	~Animation();

	Bone* FindBone(const std::string& name);

	inline std::string GetName() { return name; }
	inline float GetTicksPerSecond() { return ticksPerSecond; }
	inline float GetDuration() { return duration; }
	inline const AssimpNodeData& GetRootNode() { return rootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return boneInfoMap; }

private:
	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

private:
	float duration;
	int ticksPerSecond;
	std::vector<Bone> bones;
	AssimpNodeData rootNode;
	std::map<std::string, BoneInfo> boneInfoMap;
	std::string name;
};