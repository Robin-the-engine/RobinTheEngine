#include "rtepch.h"
#include "Mesh.h"
#include "Platform/DirectX11/DirectX11RenderSystem.h"
#include "RobinTheEngine/Application.h"
#include "RobinTheEngine/d3dUtils.h"



using namespace D3DUtils;

RTE::Mesh::Mesh( std::vector<vertex_Gouraud_shading>& vertices, std::vector<DWORD>& indices,std::vector<Texture>& textures, std::string meshName): name(meshName)
{
	RTE::DirectX11RenderSystem* rs = static_cast<RTE::DirectX11RenderSystem*>(RTE::Application::Get().GetRenderSystem());
		this->deviceContext = rs->GetContext().Get(); 
		this->textures = textures;

	ThrowIfFailed(vertexbuffer.Init(vertices.data(), vertices.size(), name));

	ThrowIfFailed(indexbuffer.Init( indices.data(), indices.size(), name));
	
}

RTE::Mesh::Mesh(const Mesh & mesh)
{
	this->deviceContext = mesh.deviceContext;
	this->indexbuffer = mesh.indexbuffer;
	this->vertexbuffer = mesh.vertexbuffer;
	this->textures = mesh.textures;
}

void RTE::Mesh::Draw()
{
	UINT offset = 0;

	for (int i = 0; i < this->textures.size(); i++)
	{
		if (this->textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE)
		{
			this->deviceContext->PSSetShaderResources(0, 1, this->textures[i].GetTextureResourceViewAddress());
			break;
		}
	}

	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexbuffer.GetAddressOf(), this->vertexbuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(this->indexbuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->DrawIndexed(this->indexbuffer.ElementCount(), 0, 0);
}

