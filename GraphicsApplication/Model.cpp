#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>


Model::Model(const std::string& path)
{
	LoadModel(path);
}

Model::Model(Mesh mesh)
{
	meshes.push_back(mesh);
}

Model::~Model()
{
	for (auto& mesh : meshes) mesh.Clear();
	meshes.clear();
}

void Model::Draw(ShaderProgram& shader)
{
	for( auto& mesh : meshes )
	{
		//mesh.ApplyMaterial(shader);
		mesh.Draw(shader);
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer import;

	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		std::cout << "ASSIMP ERROR: " << import.GetErrorString() << '\n';
	}

	directory = path.substr(0, path.find_last_of("/\\"));

	ProcessNode(scene->mRootNode, scene);
}

void Model::LoadMaterials(const std::string& path)
{
	for( auto& mesh : meshes )
	{
		mesh.LoadMaterial(path);
	}
}

void Model::ResetModel()
{
	for( auto& mesh : meshes )
	{
		mesh.Clear();
	}
	meshes.clear();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for( unsigned int i = 0; i < node->mNumMeshes; i++ )
	{
		aiMesh* inputMesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(inputMesh, scene));
	}

	for( unsigned int i = 0; i < node->mNumChildren; i++ )
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;

	for( unsigned int i = 0; i < mesh->mNumFaces; i++ )
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for( unsigned int i = 0; i < mesh->mNumVertices; i++ )
	{
		Vertex vertex;
		vertex.position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);
		vertex.normal = glm::vec4(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 0);

		if( mesh->mTextureCoords[0] )
		{
			vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
		}
		else vertex.texCoord = glm::vec2(0);

		if( mesh->HasTangentsAndBitangents() )
		{
			vertex.tangent = glm::vec4(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1);
		}
		vertices.push_back(vertex);
	}

	//if( !mesh->HasTangentsAndBitangents() ) Mesh::CalculateTangents(vertices.data(), numVertices, indices);

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuseTex");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specularTex");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "normalTex");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "heightTex");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);

}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++ )
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for( int j = 0; j < texturesLoaded.size(); j++ )
		{
			if( std::strcmp(texturesLoaded[j].GetPath().data(), str.C_Str()) == 0 )
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if( !skip )
		{
			std::string filename = directory + "\\" + str.C_Str();
			Texture texture(filename, typeName);
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	
	return textures;
}
