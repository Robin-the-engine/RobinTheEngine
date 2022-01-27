#include "rtepch.h"
#include "Material.h"
#include <d3d11.h>
#include "RobinTheEngine/ResourceFactory.h"
#include "RobinTheEngine/Application.h"
#include "imgui.h"

/*
RTE::ColoredMaterialBase::ColoredMaterialBase() : color("DefaultMaterial"),
vs(ResourceFactory::Get().GetResource<vertexShader>("texturedVS")),
ps(ResourceFactory::Get().GetResource<pixelShader>("texturedPS"))
{
}*/


void RTE::ColoredMaterialBase::SetColor(DirectX::XMFLOAT4 colorVec)
{
	color.InitializeUniqueBuffer();
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
	color.WriteBuffer();
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
		D3D11_INPUT_ELEMENT_DESC {"TANGENT", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"BITANGENT", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"TEXCOORD",0,DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	vs.init(vsPath, layout, numElements);
	std::wstring wstr(desc.psPath.begin(), desc.psPath.end());
	ps.init(wstr);

	diffuseTexture = ResourceFactory::Get().GetResource<Texture>(desc.textureKey);

	//Create sampler description for sampler state
	CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	using namespace D3DUtils;
	auto rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	ThrowIfFailed(rs->GetDevice()->CreateSamplerState(&sampDesc, clampSampler.GetAddressOf())); //Create sampler state


	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	ThrowIfFailed(rs->GetDevice()->CreateSamplerState(&sampDesc, repeatSampler.GetAddressOf())); //Create sampler state
}

void RTE::TexturedMaterialBase::SetTexture(std::string textureKey)
{
	diffuseTexture = ResourceFactory::Get().GetResource<Texture>(textureKey);
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

	//context->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	context->PSSetSamplers(0, 1, repeatSampler.GetAddressOf());
	context->PSSetSamplers(1, 1, clampSampler.GetAddressOf());
}

RTE::MaterialType RTE::TexturedMaterialBase::GetMaterialType()
{
	return MaterialType::TexturedMaterial;
}



RTE::TexturedExtentedMaterialBase::TexturedExtentedMaterialBase(MaterialDescriptor desc)
{
	std::string vsPath = desc.vsPath;
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		D3D11_INPUT_ELEMENT_DESC {"POSITION", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"NORMAL", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"TANGENT", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"BITANGENT", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"TEXCOORD",0,DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	vs.init(vsPath, layout, numElements);
	std::wstring wstr(desc.psPath.begin(), desc.psPath.end());
	ps.init(wstr);


	SetTextureByKey(&ambientTexture, &desc.AmbientTextureKey, &AmbientTextureKey);
	SetTextureByKey(&emissiveTexture, &desc.EmissiveTextureKey, &EmissiveTextureKey);
	SetTextureByKey(&diffuseTexture, &desc.DiffuseTextureKey, &DiffuseTextureKey);
	SetTextureByKey(&specularTexture, &desc.SpecularTextureKey, &SpecularTextureKey);
	SetTextureByKey(&specularPowerTexture, &desc.SpecularPowerTextureKey, &SpecularPowerTextureKey);
	SetTextureByKey(&normalTexture, &desc.NormalTextureKey, &NormalTextureKey);
	SetTextureByKey(&bumpTexture, &desc.BumpTextureKey, &BumpTextureKey);
	SetTextureByKey(&opacityTexture, &desc.OpacityTextureKey, &OpacityTextureKey);

	cb.InitializeSharedBuffer("SharedTexturedExtentedMat");

	CB_PS_MATERIAL matDesc;
	matDesc.AmbientColor = desc.AmbientColor;
	matDesc.EmissiveColor = desc.EmissiveColor;
	matDesc.DiffuseColor = desc.DiffuseColor;
	matDesc.SpecularColor = desc.SpecularColor;
	matDesc.Reflectance = desc.Reflectance;

	matDesc.Opacity = desc.Opacity;
	matDesc.SpecularPower = desc.SpecularPower;
	matDesc.IndexOfRefraction = desc.IndexOfRefraction;
	matDesc.BumpIntensity = desc.BumpIntensity;
	matDesc.SpecularScale = desc.SpecularScale;
	matDesc.AlphaThreshold = desc.AlphaThreshold;

	*((bool*)(&matDesc.HasAmbientTexture)) = isKeyNotNull(&AmbientTextureKey);
	*((bool*)(&matDesc.HasEmissiveTexture)) = isKeyNotNull(&EmissiveTextureKey);
	*((bool*)(&matDesc.HasDiffuseTexture)) = isKeyNotNull(&DiffuseTextureKey);
	*((bool*)(&matDesc.HasSpecularTexture)) = isKeyNotNull(&SpecularTextureKey);
	*((bool*)(&matDesc.HasSpecularPowerTexture)) = isKeyNotNull(&SpecularPowerTextureKey);
	*((bool*)(&matDesc.HasNormalTexture)) = isKeyNotNull(&NormalTextureKey);
	*((bool*)(&matDesc.HasBumpTexture)) = isKeyNotNull(&BumpTextureKey);
	*((bool*)(&matDesc.HasOpacityTexture)) = isKeyNotNull(&OpacityTextureKey);

	cb.data = matDesc;

	//Create sampler description for sampler state
	CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	using namespace D3DUtils;
	auto rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	ThrowIfFailed(rs->GetDevice()->CreateSamplerState(&sampDesc, clampSampler.GetAddressOf())); //Create sampler state


	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	ThrowIfFailed(rs->GetDevice()->CreateSamplerState(&sampDesc, repeatSampler.GetAddressOf())); //Create sampler state
}

void RTE::TexturedExtentedMaterialBase::ApplyMaterial()
{
	auto rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	auto context = rs->GetContext();

	context->IASetInputLayout(vs.GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(vs.GetShader(), 0, 0);
	context->PSSetShader(ps.GetShader(), 0, 0);
	cb.WriteBuffer();
	context->PSSetConstantBuffers(2, 1, cb.GetAddressOf());

	if ((bool)cb.data.HasAmbientTexture)
		context->PSSetShaderResources(0, 1, ambientTexture.GetTextureResourceViewAddress());

	if ((bool)cb.data.HasEmissiveTexture)
		context->PSSetShaderResources(1, 1, emissiveTexture.GetTextureResourceViewAddress());

	if ((bool)cb.data.HasDiffuseTexture)
		context->PSSetShaderResources(2, 1, diffuseTexture.GetTextureResourceViewAddress());

	if ((bool)cb.data.HasSpecularTexture)
		context->PSSetShaderResources(3, 1, specularTexture.GetTextureResourceViewAddress());

	if ((bool)cb.data.HasSpecularPowerTexture)
		context->PSSetShaderResources(4, 1, specularPowerTexture.GetTextureResourceViewAddress());

	if ((bool)cb.data.HasNormalTexture)
		context->PSSetShaderResources(5, 1, normalTexture.GetTextureResourceViewAddress());

	if ((bool)cb.data.HasBumpTexture)
		context->PSSetShaderResources(6, 1, bumpTexture.GetTextureResourceViewAddress());

	if ((bool)cb.data.HasOpacityTexture)
		context->PSSetShaderResources(7, 1, opacityTexture.GetTextureResourceViewAddress());


	context->PSSetSamplers(0, 1, repeatSampler.GetAddressOf());
	context->PSSetSamplers(1, 1, clampSampler.GetAddressOf());
}

RTE::MaterialType RTE::TexturedExtentedMaterialBase::GetMaterialType()
{
	return MaterialType::TexturedExtendedMaterial;
}

void RTE::TexturedExtentedMaterialBase::DrawImGui()
{

	ImGui::DragFloat3("AmbientColor", &cb.data.AmbientColor.x, 0.1f);
	ImGui::DragFloat3("EmissiveColor", &cb.data.EmissiveColor.x, 0.1f);
	ImGui::DragFloat3("DiffuseColor", &cb.data.DiffuseColor.x, 0.1f);
	ImGui::DragFloat3("SpecularColor", &cb.data.SpecularColor.x, 0.1f);
	//ImGui::DragFloat3("Reflectance", &cb.data.Reflectance.x, 0.1f);

	ImGui::DragFloat("Opacity", &cb.data.Opacity, 0.1f);
	ImGui::DragFloat("SpecularPower", &cb.data.SpecularPower, 0.1f);
	//ImGui::DragFloat("IndexOfRefraction", &cb.data.IndexOfRefraction, 0.1f);

	ImGui::Checkbox("Use AmbientTexture texture", (bool*)&cb.data.HasAmbientTexture);
	ImGui::Checkbox("Use EmissiveTexture texture", (bool*)&cb.data.HasEmissiveTexture);
	ImGui::Checkbox("Use DiffuseTexture texture", (bool*)&cb.data.HasDiffuseTexture);
	ImGui::Checkbox("Use SpecularTexture texture", (bool*)&cb.data.HasSpecularTexture);
	ImGui::Checkbox("Use SpecularPowerTexture texture", (bool*)&cb.data.HasSpecularPowerTexture);
	ImGui::Checkbox("Use NormalTexture texture", (bool*)&cb.data.HasNormalTexture);
	ImGui::Checkbox("Use BumpTexture texture", (bool*)&cb.data.HasBumpTexture);
	ImGui::Checkbox("Use OpacityTexture texture", (bool*)&cb.data.HasOpacityTexture);


	ImGui::DragFloat("BumpIntensity", &cb.data.BumpIntensity, 0.1f);
	ImGui::DragFloat("SpecularScale", &cb.data.SpecularScale, 0.1f);
	ImGui::DragFloat("AlphaThreshold", &cb.data.AlphaThreshold, 0.1f);


}

void RTE::TexturedExtentedMaterialBase::SetTextureByKey(Texture* txt, std::string* key, std::string* localKey)
{
	if ((*key) != "") {
		*localKey = (*key);
		*txt = ResourceFactory::Get().GetResource<Texture>(*key);
	}

}

bool RTE::TexturedExtentedMaterialBase::isKeyNotNull(std::string* key)
{
	return (*key) != "";
}
