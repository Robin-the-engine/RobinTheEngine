#include "rtepch.h"
#include "Material.h"
#include <d3d11.h>
#include "RobinTheEngine/ResourceFactory.h"
#include "RobinTheEngine/Application.h"


/*
RTE::ColoredMaterialBase::ColoredMaterialBase() : color("DefaultMaterial"),
vs(ResourceFactory::Get().GetResource<vertexShader>("texturedVS")),
ps(ResourceFactory::Get().GetResource<pixelShader>("texturedPS"))
{
}*/


void RTE::ColoredMaterialBase::SetColor(DirectX::XMFLOAT4 colorVec)
{

	this->color.data.vec = colorVec;

}

void RTE::ColoredMaterialBase::ApplyMaterial()
{
	auto rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	auto context = rs->GetContext();

	context->IASetInputLayout(vs.GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(vs.GetShader(), 0, 0);
	context->PSSetShader(ps.GetShader(), 0, 0);

	context->PSSetConstantBuffers(1, 1, color.GetAddressOf());

}

RTE::MaterialType RTE::ColoredMaterialBase::GetMaterialType()
{
	return MaterialType::ColoredMaterial;
}

RTE::TexturedMaterialBase::TexturedMaterialBase(MaterialDescriptor desc)
{
	std::string vsPath = desc.vsPath;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		D3D11_INPUT_ELEMENT_DESC {"POSITION", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"NORMAL", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"TEXCOORD",0,DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	vs.init(vsPath, layout, numElements);
	std::wstring wstr(desc.psPath.begin(), desc.psPath.end());
	ps.init(wstr);

	diffuseTexture = ResourceFactory::GetResource<Texture>(desc.textureKey);

}

void RTE::TexturedMaterialBase::SetTexture(std::string textureKey)
{
	diffuseTexture = ResourceFactory::GetResource<Texture>(textureKey);
}

void RTE::TexturedMaterialBase::ApplyMaterial()
{

	auto rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	auto context = rs->GetContext();

	context->IASetInputLayout(vs.GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(vs.GetShader(), 0, 0);
	context->PSSetShader(ps.GetShader(), 0, 0);

	context->PSSetShaderResources(0, 1, diffuseTexture.GetTextureResourceViewAddress());

}

RTE::MaterialType RTE::TexturedMaterialBase::GetMaterialType()
{
	return MaterialType::TexturedMaterial;
}
