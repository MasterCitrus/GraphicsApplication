#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	void InitialiseQuad();

	virtual void Draw();

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};