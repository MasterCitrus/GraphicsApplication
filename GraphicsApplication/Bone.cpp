#include "Bone.h"
#include "Utils.h"

#include <assimp/scene.h>

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
	: name(name), ID(ID), localTransform(1.0f)
{
	numPositions = channel->mNumPositionKeys;
	for( int positionIndex = 0; positionIndex < numPositions; ++positionIndex )
	{
		float timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = GetGLMVec(channel->mPositionKeys[positionIndex].mValue);
		data.timeStamp = timeStamp;
		positions.push_back(data);
	}

	numRotations = channel->mNumRotationKeys;
	for( int rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex )
	{
		float timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.orientation = GetGLMQuat(channel->mRotationKeys[rotationIndex].mValue);
		data.timeStamp = timeStamp;
		rotations.push_back(data);
	}

	numScales = channel->mNumScalingKeys;
	for( int scaleIndex = 0; scaleIndex < numScales; ++scaleIndex )
	{
		float timeStamp = (float)channel->mScalingKeys[scaleIndex].mTime;
		KeyScale data;
		data.scale = GetGLMVec(channel->mScalingKeys[scaleIndex].mValue);
		data.timeStamp = timeStamp;
		scales.push_back(data);
	}
}

void Bone::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScale(animationTime);
	localTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(float animationTime)
{
	for( int index = 0; index < numPositions - 1; ++index )
	{
		if( animationTime < positions[index + 1].timeStamp ) return index;
	}
	assert(0);
}

int Bone::GetRotationIndex(float animationTime)
{
	for( int index = 0; index < numRotations - 1; ++index )
	{
		if( animationTime < rotations[index + 1].timeStamp ) return index;
	}
	assert(0);
}

int Bone::GetScaleIndex(float animationTime)
{
	for( int index = 0; index < numScales - 1; ++index )
	{
		if( animationTime < scales[index + 1].timeStamp ) return index;
	}
	assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if( 1 == numPositions ) return glm::translate(glm::mat4(1.0f), positions[0].position);

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, animationTime);

	glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if( 1 == numRotations )
	{
		glm::quat rotation = glm::normalize(rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp, rotations[p1Index].timeStamp, animationTime);
	
	glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation, rotations[p1Index].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScale(float animationTime)
{
	if( 1 == numScales ) return glm::scale(glm::mat4(1.0f), scales[0].scale);

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp, scales[p1Index].timeStamp, animationTime);

	glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}
