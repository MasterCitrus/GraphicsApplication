#include "Model.h"
#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>


Model::Model(const char* path)
{
	std::string temp = path;
	std::size_t start = temp.find_last_of("/\\");
	std::size_t end = temp.find_last_of(".");
	name = temp.substr(start + 1, end - start - 1);
	LoadModel(path);
	if( animations.size() > 0 )
	{
		animator = new Animator(*animations.begin());
	}
}

Model::Model(Mesh* mesh)
{
	meshes.push_back(mesh);
	if( animations.size() > 0 )
	{
		animator = new Animator(*animations.begin());
	}
}

Model::~Model()
{
	for( Mesh* mesh : meshes ) delete mesh;
	meshes.clear();

	if (animations.size() > 0)
	{
		for (Animation* anim : animations) delete anim;
		animations.clear();
		delete animator;
	}
}

void Model::Update(float delta)
{
	if( animator )
	{
		animator->UpdateAnimation(delta);
	}
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
	const aiScene* scene = import.ReadFile(temp, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		std::cout << "ASSIMP ERROR: " << import.GetErrorString() << '\n';
		return;
	}

	aiNode* rootNode = scene->mRootNode;

	for( int k = 0; k < rootNode->mNumChildren; k++ )
	{
		aiNode* node = rootNode->mChildren[k];
		for( int i = 0; i < node->mNumMeshes; i++ )
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			std::cout << mesh->mName.C_Str() << '\n';
			for( int j = 0; j < mesh->mNumBones; j++ )
			{
				aiBone* bone = mesh->mBones[j];
				std::cout << bone->mName.C_Str() << '\n';
			}
		}
	}

	ProcessNode(scene->mRootNode, scene);

	if( scene->HasAnimations() )
	{
		LoadAnimations(scene);
	}
}

void Model::LoadMaterials(const char* path)
{
	for( auto mesh : meshes )
	{
		mesh->LoadMaterial(path);
	}
}

void Model::LoadAnimations(const aiScene* scene)
{
	aiNode* root = scene->mRootNode;

	for( unsigned int i = 0; i < scene->mNumAnimations; i++ )
	{
		auto anim = scene->mAnimations[i];
		animations.push_back(ProcessAnimation(anim, root));
	}
}

void Model::LoadAnimation(const char* path)
{
	Animation* anim = new Animation(path, this);
	animations.push_back(anim);

	if( !animator )
	{
		animator = new Animator(*animations.begin());
	}
}

void Model::ResetModel()
{
	for( auto mesh : meshes )
	{
		mesh->Clear();
		delete mesh;
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

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	int numFaces = mesh->mNumFaces;
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;

	for( int i = 0; i < numFaces; i++ )
	{
		aiFace face = mesh->mFaces[i];
		for( unsigned int j = 0; j < face.mNumIndices; j++ )
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	int numVertices = mesh->mNumVertices;
	for( int i = 0; i < numVertices; i++ )
	{
		Vertex vertex;
		SetVertexToBoneDataToDefault(vertex);
		vertex.position = GetGLMVec(mesh->mVertices[i]);
		vertex.normal = GetGLMVec(mesh->mNormals[i]);

		if( mesh->mTextureCoords[0] )
		{
			vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
		}
		else vertex.texCoord = glm::vec2(0.0f);

		if( mesh->HasTangentsAndBitangents() )
		{
			vertex.tangent = glm::vec4(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1);
		}

		vertices.push_back(vertex);
	}

	if( mesh->mMaterialIndex >= 0 )
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	}

	if( !mesh->HasTangentsAndBitangents() ) Mesh::CalculateTangents(vertices.data(), numVertices, indices);

	ExtractBoneWeightForVertices(vertices, mesh, scene);

	Mesh* outMesh = new Mesh(vertices.data(), indices.data(), (unsigned int)indices.size(), numVertices);

	if( scene->HasMaterials() )
	{
		aiColor3D diffuse;
		aiColor3D specular;
		aiColor3D ambient;
		float shininess = 0.0f;

		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_SHININESS, shininess);

		outMesh->meshMaterial.Ka = { ambient.r, ambient.g, ambient.b };
		outMesh->meshMaterial.Kd = { diffuse.r, diffuse.g, diffuse.b };
		outMesh->meshMaterial.Ks = { specular.r, specular.g, specular.b };
		outMesh->meshMaterial.shininess = shininess;
	}

	return outMesh;
}

Animation* Model::ProcessAnimation(aiAnimation* animation, aiNode* node)
{
	Animation* anim = new Animation(node, animation, this);
	return anim;
}

void Model::SetVertexToBoneDataToDefault(Vertex& vertex)
{
	for( int i = 0; i < MAX_BONE_INFLUENCE; i++ )
	{
		vertex.boneIDs[i] = -1;
		vertex.boneWeights[i] = 0.0f;
	}
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for( int i = 0; i < MAX_BONE_INFLUENCE; ++i )
	{
		if( vertex.boneIDs[i] < 0 )
		{
			vertex.boneWeights[i] = weight;
			vertex.boneIDs[i] = boneID;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	auto& boneMap = boneInfoMap;
	int& boneCount = boneCounter;

	for( unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex )
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if( boneMap.find(boneName) == boneMap.end() )
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for( int weightIndex = 0; weightIndex < numWeights; ++weightIndex )
		{
			int vertexID = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexID <= vertices.size());
			SetVertexBoneData(vertices[vertexID], boneID, weight);
		}
	}
}
