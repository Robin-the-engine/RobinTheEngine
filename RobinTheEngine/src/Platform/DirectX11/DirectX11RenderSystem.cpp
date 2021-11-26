#include "rtepch.h"
#include "DirectX11RenderSystem.h"
#include "DirectXColors.h"
#include "RobinTheEngine/Core.h"
#include "dxgi1_2.h"
#include "Platform/DirectX11/Camera.h"
#include "PrimitivesBatcher.h"

using namespace D3DUtils;

RTE::DirectX11RenderSystem::DirectX11RenderSystem(HWND hwnd) :m_hMainWnd(hwnd), clearColor(1, 0, 1, 1)
{
}

RTE::DirectX11RenderSystem::~DirectX11RenderSystem()
{
	for (size_t i = 0; i < adapterList.size(); ++i) {
		ReleaseCom(adapterList[i]);
	}
}

void RTE::DirectX11RenderSystem::Init()
{

	ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&m_dxgiFactory)));

	LogAdapters();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_ClientWidth;
	sd.BufferDesc.Height = m_ClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_BackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = m_hMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	HRESULT hr = D3D11CreateDeviceAndSwapChain(adapterList[0],
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,//software driver type
		flags,//flags%
		NULL,//feature levels array
		0, //Feature levels count
		D3D11_SDK_VERSION,
		&sd,
		this->mSwapChain.GetAddressOf(),
		this->m_d3dDevice.GetAddressOf(),
		NULL,//supported feature level
		this->m_DeviceContext.GetAddressOf()
	);
	ThrowIfFailed(hr);
	//TODO: Create software adapter if failed

	OnResize(m_ClientWidth, m_ClientHeight);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	ThrowIfFailed(m_d3dDevice->CreateRasterizerState(&rasterizerDesc, m_RasterizerState.GetAddressOf()));
	m_DeviceContext->RSSetState(m_RasterizerState.Get());

	D3D11_DEPTH_STENCIL_DESC depthStateDesc;
	ZeroMemory(&depthStateDesc, sizeof(depthStateDesc));

	depthStateDesc.DepthEnable = true;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ThrowIfFailed(m_d3dDevice->CreateDepthStencilState(&depthStateDesc, m_DepthStencilState.GetAddressOf()));
	PrimitivesBatcher::Init(m_d3dDevice.Get(), m_DeviceContext.Get(), XMFLOAT4X4(), XMFLOAT4X4());
}

void RTE::DirectX11RenderSystem::OnResize(int width, int height)
{
	m_ClientWidth = width;
	m_ClientHeight = height;

	RTE_ASSERT(m_d3dDevice, "Bad device!");
	RTE_ASSERT(mSwapChain, "Bad swapchain!");

	// Release the previous resources we will be recreating.
	for (int i = 0; i < SwapChainBufferCount; ++i)
		m_SwapChainBuffer[i].Reset();
	m_DepthStencilView.Reset();

	m_RenderTargetView.Reset();
	m_DepthStencilView.Reset();
	// Resize the swap chain.
	ThrowIfFailed(mSwapChain->ResizeBuffers(
		SwapChainBufferCount,
		m_ClientWidth, m_ClientHeight,
		m_BackBufferFormat,
		0));

	ComPtr<ID3D11Texture2D> backbuffer;
	ThrowIfFailed(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backbuffer.GetAddressOf())));
	ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backbuffer.Get(), NULL, m_RenderTargetView.GetAddressOf()));

	mCurrBackBuffer = 0;

	// Create the depth/stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_ClientWidth;
	depthStencilDesc.Height = m_ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
	ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, DepthStencilBuffer.GetAddressOf()));
	ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), 0, m_DepthStencilView.GetAddressOf()));


	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

	// Update the viewport transform to cover the client area.
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
	m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &m_ScreenViewport);


	m_ScissorRect = { 0, 0, m_ClientWidth, m_ClientHeight };
	m_DeviceContext->RSSetScissorRects(1, &m_ScissorRect);

	if (mainCamera != nullptr)
		mainCamera->SetProjectionProperties(90, width / height, 0.05, 1000);

}

void RTE::DirectX11RenderSystem::OnRenderBegin()
{

	//	// Clear the back buffer and depth buffer.
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), &clearColor.x);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);

	frameStats.ObjectsWasDrawed = 0;
}

void RTE::DirectX11RenderSystem::OnRenderEnd()
{

	//	// swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));

}

void RTE::DirectX11RenderSystem::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring tmp = desc.DeviceName;
		std::string tmp2(tmp.begin(), tmp.end());
		std::string text = "***Output: ";
		text += std::string(tmp.begin(), tmp.end());
		RTE_CORE_INFO(text);

		LogOutputDisplayModes(output, m_BackBufferFormat);

		ReleaseCom(output);

		++i;
	}
}

void RTE::DirectX11RenderSystem::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	adapterList;
	while (m_dxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{

		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		std::wstring tmp = desc.Description;
		std::string text = " * **Adapter: ";
		text += std::string(tmp.begin(), tmp.end());
		RTE_CORE_INFO(text);
		adapterList.push_back(adapter);    ++i;
	}
#ifdef _DEBUG
	for (size_t i = 0; i < adapterList.size(); ++i) {

		LogAdapterOutputs(adapterList[i]);
	}
#endif // DEBUG
}


void RTE::DirectX11RenderSystem::SetCamera(Camera* camera)
{
	mainCamera = camera;
}


/*
void RTE::DirectX11RenderSystem::Draw(GameObject go)
{
	static ConstantBuffer<CB_VS_WORLD_MAT> world;
	static bool flag = true;
	if (flag) {
		world.InitializeSharedBuffer("worldMat");
		flag = false;
	}

	auto mr = go.GetComponent<RTE::MeshRenderer>();
	mr.GetMaterial().matPtr->ApplyMaterial();
	mr.GetMesh().meshes[0]->BindMesh(m_DeviceContext.Get());
	auto trans = go.GetComponent<RTE::Transform>();
	XMStoreFloat4x4(&world.data.worldMatrix, trans.GetMatrix());
	auto mvp = trans.GetMatrix() * mainCamera->GetViewMatrix() * mainCamera->GetProjectionMatrix();
	XMFLOAT4X4 mvpStored; XMStoreFloat4x4(&mvpStored, mvp);
	world.WriteBuffer();
	mainCamera->UpdateBuffer();
	//set camera
	m_DeviceContext->VSSetConstantBuffers(0, 1, mainCamera->constBuffer.GetAddressOf());
	//set world
	m_DeviceContext->VSSetConstantBuffers(1, 1, world.GetAddressOf());

	m_DeviceContext->DrawIndexed(mr.GetMesh().meshes[0]->elementCount, 0, 0);

}
*/

void RTE::DirectX11RenderSystem::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::string text =
			"Width = " + std::to_string(x.Width) + " " +
			"Height = " + std::to_string(x.Height) + " " +
			"Refresh = " + std::to_string(n) + "/" + std::to_string(d);

		RTE_CORE_INFO(text);
	}
}

void RTE::DirectX11RenderSystem::DoRender(std::tuple<RTE::Transform, RTE::MeshRenderer> mesh)
{
	using namespace DirectX;
	static ConstantBuffer<CB_VS_WORLD_MAT> world;
	static bool flag = true;

	if (flag) {
		world.InitializeSharedBuffer("worldMat");
		flag = false;
		BoundingFrustum::CreateFromMatrix(frameFrustrum, mainCamera->GetProjectionMatrix());

	}

	auto& mr = std::get<1>(mesh);
	mr.GetMaterial().matPtr->ApplyMaterial();
	mr.GetMesh().meshes[0]->BindMesh(m_DeviceContext.Get());




	//get Transform
	auto& transform = std::get<0>(mesh);
	auto worldMatrix = transform.GetMatrix();
	XMStoreFloat4x4(&world.data.worldMatrix, XMMatrixTranspose(worldMatrix));

	auto tmpVec = XMMatrixDeterminant(worldMatrix);
	XMMATRIX invWorld = XMMatrixInverse(&tmpVec, worldMatrix);
	tmpVec = XMMatrixDeterminant(mainCamera->GetViewMatrix());
	XMMATRIX invView = XMMatrixInverse(&tmpVec, mainCamera->GetViewMatrix());
	XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);
	DirectX::BoundingFrustum localSpaceFrustrum;
	frameFrustrum.Transform(localSpaceFrustrum, viewToLocal);

	if (localSpaceFrustrum.Contains(mr.GetMesh().box) == DirectX::DISJOINT || (frustrumCullingEnabled == false))
		return;


	world.WriteBuffer();
	mainCamera->UpdateBuffer();
	//set camera
	m_DeviceContext->VSSetConstantBuffers(0, 1, mainCamera->constBuffer.GetAddressOf());
	//set world
	m_DeviceContext->VSSetConstantBuffers(1, 1, world.GetAddressOf());

	m_DeviceContext->DrawIndexed(mr.GetMesh().meshes[0]->elementCount, 0, 0);

	XMFLOAT4X4 tmpView; XMStoreFloat4x4(&tmpView, mainCamera->GetViewMatrix());
	XMFLOAT4X4 tmpProjection; XMStoreFloat4x4(&tmpProjection, mainCamera->GetProjectionMatrix());
	PrimitivesBatcher::SetViewProjection(tmpView, tmpProjection);
	BoundingBox proverka = mr.GetMesh().box;
	BoundingBox tmpBox; mr.GetMesh().box.Transform(tmpBox, worldMatrix);
	PrimitivesBatcher::DrawPrimitive(m_DeviceContext.Get(), tmpBox);
	PrimitivesBatcher::DrawGrid(m_DeviceContext.Get());

	frameStats.ObjectsWasDrawed++;
}

void RTE::DirectX11RenderSystem::DoRender(std::tuple<RTE::Transform, RTE::MeshRenderer> meshes, void* lightComps)
{
	RTE::DirectX11RenderSystem::DoRender(meshes);
}



