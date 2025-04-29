#version 460 core

layout( location = 0) in vec3 Position;
layout( location = 1) in vec3 Normal;
layout( location = 2) in vec2 TexCoords;
layout( location = 3) in vec4 Tangent;
layout( location = 4) in ivec4 boneIDs;
layout( location = 5) in vec4 weights;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vTangent;
out vec3 vBiTangent;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBoneMatrices[MAX_BONES];

uniform mat4 ProjectionViewModel;

uniform mat4 ModelMatrix;

void main() 
{
	vec4 totalPosition = vec4(0.0f);
	vec3 totalNormal = vec3(0.0f);
	for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if(boneIDs[i] == -1) continue;
		if(boneIDs[i] >= MAX_BONES)
		{
			totalPosition = vec4(Position, 1.0f);
			break;
		}
		vec4 localPosition = finalBoneMatrices[boneIDs[i]] * vec4(Position, 1.0f);
		totalPosition += localPosition * weights[i];
		vec3 localNormal = mat3(finalBoneMatrices[boneIDs[i]]) * Normal;
		totalNormal += localNormal * weights[i];
	}
	
	totalNormal = normalize(totalNormal);

	vPosition = ModelMatrix * totalPosition;
	vNormal = (ModelMatrix * vec4(totalNormal, 0.0)).xyz;
	vTexCoords = TexCoords;
	vTangent = (ModelMatrix * vec4(Tangent.xyz, 0)).xyz;
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;
	gl_Position = ProjectionViewModel * totalPosition;
}