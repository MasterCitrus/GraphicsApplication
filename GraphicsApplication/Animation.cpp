#include "Animation.h"
#include "Model.h"
#include "Bone.h"
#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Animation::Animation(aiNode* node, aiAnimation* animation, Model* model)
{
	duration = animation->mDuration;
	ticksPerSecond = animation->mTicksPerSecond;

	//aiMatrix4x4 globalTransformation = node->mTransformation;
	//globalTransformation = globalTransformation.Inverse();

	name = animation->mName.C_Str();
	name = name.substr(name.find_last_of("|") + 1);

	ReadHierarchyData(rootNode, node);
	ReadMissingBones(animation, *model);
}

Animation::Animation(const std::string& animationPath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);

	auto animation = scene->mAnimations[0];

	name = animation->mName.C_Str();
	name = name.substr(name.find_last_of("|") + 1);

	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();

	duration = animation->mDuration;
	ticksPerSecond = animation->mTicksPerSecond;
	ReadHierarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Animation::~Animation()
{
}

Bone* Animation::FindBone(const std::string& name)
{
	auto it = std::find_if(bones.begin(), bones.end(), [&](const Bone& bone)
		{
			return bone.GetBoneName() == name;
		});

	if( it == bones.end() ) return nullptr;
	else return &( *it );
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;

	auto& boneInfoMap = model.GetBoneInfoMap();
	int& boneCount = model.GetBoneCount();

	for( int i = 0; i < size; i++ )
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if( boneInfoMap.find(boneName) == boneInfoMap.end() )
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
	}

	this->boneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for( int i = 0; i < src->mNumChildren; i++ )
	{
		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}
