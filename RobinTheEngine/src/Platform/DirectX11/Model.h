#pragma once
#include "Platform/DirectX11/Mesh.h"
#include <string>

using namespace DirectX;

namespace RTE {


	class Model
	{
	public:
		bool Initialize(const std::string& path, ConstantBuffer<CB_VS_MATRIX4x4>& cb_vs_vertexshader);
		void SetTexture(ID3D11ShaderResourceView * texture);
		void Draw(const XMMATRIX& worldMatrix, const XMMATRIX & viewProjectionMatrix);
		std::vector<Mesh> meshes;


	private:

		bool LoadModel(const std::string & filePath);
		void ProcessNode(aiNode * node, const aiScene * scene);
		Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
		void GetDirectoryFromPath(const std::string& path);

		std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
		int GetTextureIndex(aiString* pStr);

		ID3D11Device * device = nullptr;
		ID3D11DeviceContext * deviceContext = nullptr;
		ConstantBuffer<CB_VS_MATRIX4x4> * cb_vs_vertexshader = nullptr;
		ID3D11ShaderResourceView * texture = nullptr;

		std::string directory = "";

	};






}




