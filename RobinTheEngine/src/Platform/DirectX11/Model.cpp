#include "rtepch.h"
#include "Model.h"
#include "RobinTheEngine/d3dUtils.h"
#include "Platform/DirectX11/DirectX11RenderSystem.h"
#include "RobinTheEngine/Application.h"


#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <unordered_map>

using namespace D3DUtils;


RTE::Model* RTE::Model::CreateModel(const std::string& path, int layout)
{
	Model* m = new Model();
	if (!m->LoadModel(path, layout)) {
		std::string warn = "Cant load model with path: " + path;
		RTE_CORE_WARN(warn);
	}
	return m;
}

bool RTE::Model::LoadModel(const std::string& filePath, int layout)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded);

	if (pScene == NULL)
		return false;


	this->ProcessNode(pScene->mRootNode, pScene, layout);
	return true;
}

void RTE::Model::ProcessNode(aiNode* node, const aiScene* scene, int layout)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->ProcessMesh(mesh, layout));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene, layout);
	}
}

std::shared_ptr<RTE::IMesh> processDefaultMesh(aiMesh* mesh) {
	// Data to fill
	std::vector<RTE::vertex_pos_color> vertices;
	std::vector<DWORD> indices;

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		RTE::vertex_pos_color vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		vertices.push_back(vertex);
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	using namespace RTE;
	//std::shared_ptr<RTE::IMeshTwo> meshPtr = std::make_shared<RTE::MeshTwo<RTE::vertex_pos_color>>(vertices, indices);
	std::shared_ptr<IMesh> meshPtr = std::shared_ptr<Mesh<vertex_pos_color>>(new Mesh<vertex_pos_color>(vertices, indices));
	return meshPtr;
}

std::shared_ptr<RTE::IMesh> processTexturedMesh(aiMesh* mesh) {
	// Data to fill
	std::vector<RTE::vertex_Gouraud_shading> vertices;
	std::vector<DWORD> indices;

	//Get vertices
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		RTE::vertex_Gouraud_shading vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	//Get indices
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	std::shared_ptr<RTE::IMesh> meshPtr = std::make_shared<RTE::Mesh<RTE::vertex_Gouraud_shading>>(vertices, indices);
	return meshPtr;
}

std::shared_ptr<RTE::IMesh> RTE::Model::ProcessMesh(aiMesh* mesh, int layout)
{
	//UNDONE: place for adding mesh processing functions for new mesh layouts
	switch (layout)
	{
	case (1):
		return processDefaultMesh(mesh);
		break;
	case (2):
		return processTexturedMesh(mesh);
		break;
	default:
		RTE_CORE_ASSERT(false, "Bad layout index");
		//return std::make_shared<RTE::IMeshTwo>(nullptr);
		break;
	}
}