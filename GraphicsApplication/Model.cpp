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
		delete animator;
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
	const aiScene* scene = import.ReadFile(temp, aiProcess_Triangulate);

	if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		std::cout << "ASSIMP ERROR: " << import.GetErrorString() << '\n';
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
		animations.push_back(ProcessAnimation(scene->mAnimations[i], root));
	}
}

void Model::LoadAnimation(const char* path)
{
	Animation* anim = new Animation(path, this);
	animations.push_back(anim);
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
	for( int i = 0; i < numVertices; i++ )
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

	if( mesh->mMaterialIndex >= 0 )
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	}

	if( !mesh->HasTangentsAndBitangents() ) Mesh::CalculateTangents(vertices.data(), numVertices, indices);

	ExtractBoneWeightForVertices(vertices, mesh, scene);

	Mesh* outMesh = new Mesh(vertices.data(), indices.data(), indices.size(), numVertices);

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
	for( unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex )
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if( boneInfoMap.find(boneName) == boneInfoMap.end() )
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCounter;
			newBoneInfo.offset = ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCounter;
			boneCounter++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
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
