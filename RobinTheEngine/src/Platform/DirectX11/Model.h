#pragma once
#include "Platform/DirectX11/Mesh.h"
#include <string>
#include <memory>
#include "RobinTheEngine/Scene/BaseResource.h"
#include <DirectXCollision.h>


#include "assimp/scene.h"

using namespace DirectX;

namespace RTE {

	class Model: public BaseResource
	{
	public:
		Model():BaseResource("invalid"){}
		bool Initialize(const std::string& path, int layout);
		std::vector<std::shared_ptr<IMesh>> meshes;
		DirectX::BoundingBox box;

		static Model* CreateModel(const std::string& path, int layout);


	private:
		bool LoadModel(const std::string& filePath, int lauout);
		void ProcessNode(aiNode* node, const aiScene* scene, int layout);
		std::shared_ptr<IMesh> ProcessMesh(aiMesh* mesh, int layout);
		void GetBoundingCoords();

		DirectX::XMFLOAT3 minCoords = XMFLOAT3(0, 0, 0);
		DirectX::XMFLOAT3 maxCoords = XMFLOAT3(0, 0, 0);
		
	};






	/*	class Model : public IMesh
	{
	public:
		bool Initialize(const std::string& path, ConstantBuffer<CB_VS_MATRIX4x4>& cb_vs_vertexshader);
		void SetTexture(ID3D11ShaderResourceView* texture);
		//void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);
		void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix) override;
		std::vector<Mesh> meshes;
		Model() = default;

	private:

		bool LoadModel(const std::string& filePath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void GetDirectoryFromPath(const std::string& path);

		std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
		int GetTextureIndex(aiString* pStr);

		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* deviceContext = nullptr;
		ConstantBuffer<CB_VS_MATRIX4x4>* cb_vs_vertexshader = nullptr;
		ID3D11ShaderResourceView* texture = nullptr;

		std::string directory = "";

	};*/


}




