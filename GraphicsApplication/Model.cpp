#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>


Model::Model(const char* path)
{
	LoadModel(path);
}

Model::~Model()
{
	meshes.clear();
}

void Model::Draw(ShaderProgram* shader)
{
	for( auto mesh : meshes )
	{
		mesh->ApplyMaterial(shader);
		mesh->Draw();
	}
}

void Model::LoadModel(const char* path)
{
	Assimp::Importer import;

	std::string temp = path;
	//aiImportFile(path, 0)
	const aiScene* scene = import.ReadFile(temp, aiProcess_Triangulate);

	ProcessNode(scene->mRootNode, scene);
}

void Model::LoadMaterials(const char* path)
{
	for( auto mesh : meshes )
	{
		mesh->LoadMaterial(path);
	}
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for( unsigned int i = 0; i < node->mNumMeshes; i++ )
	{
		aiMesh* inputMesh = scene->mMeshes[i];
		meshes.push_back(ProcessMesh(inputMesh, scene));
	}

	for( unsigned int i = 0; i < node->mNumChildren; i++ )
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indices;

	for( int i = 0; i < numFaces; i++ )
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);

		if( mesh->mFaces[i].mNumIndices == 4 )
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[3]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

	int numVertices = mesh->mNumVertices;
	Mesh::Vertex* vertices = new Mesh::Vertex[numVertices];
	for( int i = 0; i < numVertices; i++ )
	{
		vertices[i].position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
		vertices[i].normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);

		if( mesh->mTextureCoords[0] )
		{
			vertices[i].texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
		}
		else vertices[i].texCoord = glm::vec2(0);

		if( mesh->HasTangentsAndBitangents() )
		{
			vertices[i].tangent = glm::vec4(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1);
		}
	}

	if( !mesh->HasTangentsAndBitangents() ) Mesh::CalculateTangents(vertices, numVertices, indices);

	Mesh* outMesh = new Mesh(vertices, indices.data(), indices.size(), numVertices);

	return outMesh;
}
