#include "rtepch.h"
#include "FrameBuffer.h"
#include "RobinTheEngine/d3dUtils.h"

void RTE::FrameBuffer::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, int textureWidth, int textureHeight)
{
	using namespace D3DUtils;
	RTE_CORE_ASSERT(!initializedFlag, "That framebuffer already initialized!");

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ThrowIfFailed(device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	ThrowIfFailed(device->CreateRenderTargetView(m_renderTargetTexture.Get(), &renderTargetViewDesc, m_renderTargetView.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	ThrowIfFailed( device->CreateShaderResourceView(m_renderTargetTexture.Get() , &shaderResourceViewDesc, m_shaderResourceView.GetAddressOf()));
	initializedFlag = true;
}

void RTE::FrameBuffer::SetRenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencil)
{
	context->OMSetRenderTargets(1, this->m_renderTargetView.GetAddressOf(), depthStencil.Get());
}

void RTE::FrameBuffer::ClearRenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencil, DirectX::XMFLOAT4 color)
{
	context->ClearRenderTargetView(m_renderTargetView.Get(), &color.x);
	context->ClearDepthStencilView(depthStencil.Get(), D3D11_CLEAR_DEPTH, 1, 0);
}

void RTE::FrameBuffer::Resize(Microsoft::WRL::ComPtr<ID3D11Device> device, int textureWidth, int textureHeight)
{
	m_renderTargetTexture.Reset();
	m_renderTargetView.Reset();
	m_shaderResourceView.Reset();
	initializedFlag = false;
	Initialize(device, textureWidth, textureHeight);
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> RTE::FrameBuffer::GetShaderResourceView()
{
	return m_shaderResourceView;
}
