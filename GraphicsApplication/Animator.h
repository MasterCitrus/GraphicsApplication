#pragma once

#include <glm/glm.hpp>

#include <vector>

class Animation;

struct AssimpNodeData;

class Animator
{
public:
	Animator(Animation* animation);

	void UpdateAnimation(float delta);
	void PlayAnimation(Animation* animation);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<glm::mat4> finalBoneMatrices;
	Animation* currentAnimation;
	float currentTime;
	float deltaTime;
};