#pragma once
#include "Texture.h"
#include "MAterial.h"
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

namespace aie { class ShaderProgram; }

class Mesh
{
public:
	Material meshMaterial;

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
		glm::vec4 tangent;
	};

	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	Mesh(Vertex* vertices, unsigned int* indices, unsigned int indexCount, unsigned int vertexCount);
	virtual ~Mesh();


	void ApplyMaterial(aie::ShaderProgram* shader);
	void LoadMaterial(const char* filename);

	void InitialiseFromFile(const char* filename);
	void Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	void InitialiseQuad();

	static void CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices);

	virtual void Draw();

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};