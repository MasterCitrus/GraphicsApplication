#include "Mesh.h"
#include "Shader.h"
#include <glad/glad.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <fstream>
#include <string>
#include <sstream>

Mesh::Mesh(Vertex* vertices, unsigned int* indices, unsigned int indexCount, unsigned int vertexCount) : triCount(0), vao(0), vbo(0), ibo(0)
{
	InitialiseOld(vertexCount, vertices, indexCount, indices);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) 
	: triCount(0), vao(0), vbo(0), ibo(0),
	textures(std::move(textures)), vertices(std::move(vertices)), indices(std::move(indices))
{
	Initialise();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::ApplyMaterial(aie::ShaderProgram& shader)
{
	shader.bindUniform("Ka", meshMaterial.Ka);
	shader.bindUniform("Kd", meshMaterial.Kd);
	shader.bindUniform("Ks", meshMaterial.Ks);
	shader.bindUniform("specularPower", meshMaterial.shininess);

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for( int i = 0; i < textures.size(); i++ )
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].GetType();
		if( name == "diffuseTex" ) number = std::to_string(diffuseNr++);
		else if( name == "specularTex" ) number = std::to_string(specularNr++);
		else if( name == "normalTex" ) number = std::to_string(normalNr++);
		else if( name == "heightTex" ) number = std::to_string(heightNr++);

		shader.bindUniform((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].GetTextureID());
	}

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::LoadMaterial(const std::string& path)
{
	std::fstream file(path, std::ios::in);
	std::string line;
	std::string header;
	char buffer[256];

	std::string directory(path);
	size_t index = directory.find_last_of("/\\");
	if (index != -1)
	{
		directory = directory.substr(0, index + 1);
	}

	while (!file.eof())
	{
		file.getline(buffer, 256);
		line = buffer;
		std::stringstream ss(line, std::stringstream::in | std::stringstream::out);

		if (line.find("Ka") == 0) ss >> header >> meshMaterial.Ka.x >> meshMaterial.Ka.y >> meshMaterial.Ka.z;
		else if (line.find("Kd") == 0) ss >> header >> meshMaterial.Kd.x >> meshMaterial.Kd.y >> meshMaterial.Kd.z;
		else if (line.find("Ks") == 0) ss >> header >> meshMaterial.Ks.x >> meshMaterial.Ks.y >> meshMaterial.Ks.z;
		else if (line.find("Ns") == 0) ss >> header >> meshMaterial.shininess;
	}
}

void Mesh::InitialiseFromFile(const char* filename)
{
	const aiScene* scene = aiImportFile(filename, 0);

	aiMesh* mesh = scene->mMeshes[0];

	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indices;
	for (int i = 0; i < numFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);

		if (mesh->mFaces[i].mNumIndices == 4)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[3]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	int numVertices = mesh->mNumVertices;
	Vertex* vertices = new Vertex[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
		vertices[i].normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);
		
		if (mesh->mTextureCoords[0])
		{
			vertices[i].texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
		}
		else vertices[i].texCoord = glm::vec2(0);

		if (mesh->HasTangentsAndBitangents())
		{
			vertices[i].tangent = glm::vec4(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1);
		}
	}

	if (!mesh->HasTangentsAndBitangents()) CalculateTangents(vertices, numVertices, indices);

	InitialiseOld(numVertices, vertices, (unsigned int)indices.size(), indices.data());

	delete[] vertices;

}

void Mesh::Initialise()
{
	assert(vao == 0);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	triCount = (unsigned int)indices.size() / 3;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glBindVertexArray(0);
}

void Mesh::InitialiseOld(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	assert(vao == 0);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	if( indexCount != 0 )
	{
		glGenBuffers(1, &ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else
	{
		triCount = vertexCount / 3;
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::InitialiseQuad()
{
	assert(vao == 0);

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Vertex vertices[4];
	vertices[0].position = { 0.5f, 0.0f, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0.0f, -0.5f, 1 };
	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1 };
	vertices[3].position = { -0.5f, 0.0f, 0.5f, 1 };

	vertices[0].normal = { 0, 1, 0, 0};
	vertices[1].normal = { 0, 1, 0, 0};
	vertices[2].normal = { 0, 1, 0, 0};
	vertices[3].normal = { 0, 1, 0, 0};

	vertices[0].texCoord = { 0.0f, 1.0f };
	vertices[1].texCoord = { 1.0f, 1.0f };
	vertices[2].texCoord = { 0.0f, 0.0f };
	vertices[3].texCoord = { 1.0f, 0.0f };

	std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };

	CalculateTangents(&vertices[0], 4, indices);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triCount = 2;
}

void Mesh::InitialiseCube()
{
	assert(vao == 0);
	
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float cubeVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), cubeVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triCount = 12;
}

void Mesh::Clear()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	triCount = 0;
}

void Mesh::CalculateTangents(Vertex* vertices, unsigned int vertexCount, const std::vector<unsigned int>& indices)
{
	glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
	glm::vec4* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

	unsigned int indexCount = (unsigned int)indices.size();
	for (unsigned int a = 0; a < indexCount; a+= 3)
	{
		long i1 = indices[a];
		long i2 = indices[a + 1];
		long i3 = indices[a + 2];

		const glm::vec4& v1 = vertices[i1].position;
		const glm::vec4& v2 = vertices[i2].position;
		const glm::vec4& v3 = vertices[i3].position;

		const glm::vec2& w1 = vertices[i1].position;
		const glm::vec2& w2 = vertices[i2].position;
		const glm::vec2& w3 = vertices[i3].position;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r, 0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r, 0);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (unsigned int a = 0; a < vertexCount; a++)
	{
		const glm::vec3& n = glm::vec3(vertices[a].normal);
		const glm::vec3& t = glm::vec3(tan1[a]);

		vertices[a].tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);

		vertices[a].tangent.w = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[a])) < 0.0f) ? 1.0f : -1.0f;
	}

	delete[] tan1;
}

void Mesh::Draw(aie::ShaderProgram& shader)
{
	shader.bindUniform("Ka", meshMaterial.Ka);
	shader.bindUniform("Kd", meshMaterial.Kd);
	shader.bindUniform("Ks", meshMaterial.Ks);
	shader.bindUniform("specularPower", meshMaterial.shininess);

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for( int i = 0; i < textures.size(); i++ )
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].GetType();
		if( name == "diffuseTex" ) number = std::to_string(diffuseNr++);
		else if( name == "specularTex" ) number = std::to_string(specularNr++);
		else if( name == "normalTex" ) number = std::to_string(normalNr++);
		else if( name == "heightTex" ) number = std::to_string(heightNr++);

		glBindTexture(GL_TEXTURE_2D, textures[i].GetTextureID());
		shader.bindUniform(( name + number ).c_str(), i);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
