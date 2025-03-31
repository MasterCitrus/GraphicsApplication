#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
	vTexCoords = TexCoords;
	vPosition = vec4(model * vec4(Position, 1.0));
	vNormal = (normalMatrix * Normal).xyz;

	gl_Position = projection * view * vec4(vPosition, 1.0);
}