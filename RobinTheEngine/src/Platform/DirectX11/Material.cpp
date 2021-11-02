#include "rtepch.h"
#include "Material.h"
#include <d3d11.h>
#include "RobinTheEngine/ResourceFactory.h"
#include "RobinTheEngine/Application.h"



RTE::DefaultMaterial::DefaultMaterial() : color("DefaultMaterial"),
vs(ResourceFactory::Get().GetResource<vertexShader>("DefaultVS")),
ps(ResourceFactory::Get().GetResource<pixelShader>("DefaultPS"))
{
}

void RTE::DefaultMaterial::SetColor(DirectX::XMFLOAT4 colorVec)
{

	this->color.data.vec = colorVec;

}

void RTE::DefaultMaterial::ApplyMaterial()
{
	auto rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	auto context = rs->GetContext();

	context->IASetInputLayout(vs.GetInputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(vs.GetShader(), 0, 0);
	context->PSSetShader(ps.GetShader(), 0, 0);

}
