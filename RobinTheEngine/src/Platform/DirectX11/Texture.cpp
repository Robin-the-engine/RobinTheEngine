#include "rtepch.h"
#include "Platform/DirectX11/Texture.h"
#include "Platform/DirectX11/DirectX11RenderSystem.h"
#include "RobinTheEngine/Application.h"

#include "WICTextureLoader.h"
#pragma comment(lib,"DirectXTK.lib")

RTE::Texture::Texture(const std::string& filePath, aiTextureType type)
{
	this->type = type;

	int x, y, n;

	if (FAILED(CreateTextureFromFile(filePath))) {
		RTE_ASSERT(CreateTextureFromFile(DEFAULT_PATH_TO_ERROR_TEXTURE),
			"Unable to create error_checker texture from assets\\textures");
	}
}


aiTextureType RTE::Texture::GetType()
{
	return this->type;
}

ID3D11ShaderResourceView * RTE::Texture::GetTextureResourceView()
{
	return this->textureView.Get();
}

ID3D11ShaderResourceView ** RTE::Texture::GetTextureResourceViewAddress()
{
	return this->textureView.GetAddressOf();
}

HRESULT RTE::Texture::CreateTextureFromFile(const std::string & path)
{
	DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	std::wstring wpath(path.begin(), path.end());
	return DirectX::CreateWICTextureFromFile(rs->GetDevice().Get(), wpath.c_str(),texture.GetAddressOf(), textureView.GetAddressOf());
}



