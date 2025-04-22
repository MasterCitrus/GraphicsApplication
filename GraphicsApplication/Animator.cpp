#include "Animator.h"

#include "Animation.h"
#include "Bone.h"

Animator::Animator(Animation* animation)
{
	currentTime = 0.0f;
	currentAnimation = animation;

	finalBoneMatrices.reserve(100);

	for( int i = 0; i < 100; i++ )
	{
		finalBoneMatrices.push_back(glm::mat4(1.0f));
	}
}

void Animator::UpdateAnimation(float delta)
{
	deltaTime = delta;
	if( currentAnimation )
	{
		currentTime += currentAnimation->GetTicksPerSecond() * delta;
		currentTime = fmod(currentTime, currentAnimation->GetDuration());
		CalculateBoneTransform(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::PlayAnimation(Animation* animation)
{
	currentAnimation = animation;
	currentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* bone = currentAnimation->FindBone(nodeName);

	if( bone )
	{
		bone->Update(currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneMapInfo = currentAnimation->GetBoneIDMap();
	if( boneMapInfo.find(nodeName) != boneMapInfo.end() )
	{
		int index = boneMapInfo[nodeName].id;
		glm::mat4 offset = boneMapInfo[nodeName].offset;
		finalBoneMatrices[index] = globalTransformation * offset;
	}

	for( int i = 0; i < node->childrenCount; i++ )
	{
		CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}
