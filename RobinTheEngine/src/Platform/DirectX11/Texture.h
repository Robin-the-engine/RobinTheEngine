#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <wrl/client.h>
#include <string>
#include <assimp/material.h>

using namespace DirectX::PackedVector;
using namespace DirectX;

namespace RTE {


	const std::string DEFAULT_PATH_TO_ERROR_TEXTURE = "\\assets\\textures\\error_checker.png";

	class Texture
	{
		
	public:
		Texture( const std::string & filePath, aiTextureType type);
		Texture(const Texture&) = default;
		aiTextureType GetType();
		ID3D11ShaderResourceView * GetTextureResourceView();
		ID3D11ShaderResourceView ** GetTextureResourceViewAddress();
	private:
		HRESULT CreateTextureFromFile(const std::string& path);
		Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
		aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;

	};




}
