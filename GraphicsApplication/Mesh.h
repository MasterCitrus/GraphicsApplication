#pragma once
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

namespace aie { class ShaderProgram; }

class Mesh
{
public:
	Material meshMaterial;

	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	Mesh(Vertex* vertices, unsigned int* indices, unsigned int indexCount, unsigned int vertexCount);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	virtual ~Mesh();


	void ApplyMaterial(aie::ShaderProgram& shader);
	void LoadMaterial(const std::string& path);

	void InitialiseFromFile(const char* filename);
	void Initialise();
	void InitialiseOld(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	void InitialiseQuad();
	void InitialiseCube();

	void Clear();

	static void CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices);

	virtual void Draw(aie::ShaderProgram& shader);

public:
	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

protected:
	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};