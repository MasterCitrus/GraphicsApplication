#pragma once
#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec4 position;
	glm::vec4 normal;
	glm::vec2 texCoord;
	glm::vec4 tangent;
	int boneIDs[MAX_BONE_INFLUENCE];
	float boneWeights[MAX_BONE_INFLUENCE];
};