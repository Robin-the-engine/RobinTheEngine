#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>

#include <D3Dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <dxgi1_4.h>

#include "RobinTheEngine/RenderSystem.h"
#include "Platform/DirectX11/FrameBuffer.h"


#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
//using namespace D3DUtils;
namespace RTE {

	//struct FrameStats;
	struct FrameStats
	{
		int ObjectsWasDrawed;
	};

	class DirectX11RenderSystem : public RTE::RenderSystem
	{
	public:
		DirectX11RenderSystem(HWND hwnd);
		~DirectX11RenderSystem();
		void Init() override;
		void OnResize(int width, int height) override;
		void OnRenderBegin() override;
		void OnRenderEnd() override;
		void LogAdapters();
		DirectX::XMFLOAT4& GetClearColor() { return clearColor; }


		Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return m_d3dDevice; }
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return m_DeviceContext; }
		void SetCustomFrameBuffer();


		void SetCamera(Camera* camera) override;
		//void Draw(GameObject go);

		virtual void DoRender(std::tuple<RTE::Transform, RTE::MeshRenderer> meshes) override;
		virtual void DoRender(std::tuple<RTE::Transform, RTE::MeshRenderer> meshes, void* lightComps) override;
		FrameStats& GetFrameStats() { return frameStats; }
		FrameBuffer* GetFrameBufferPtr() { return &framebufferForTexture; }
		void SetDefaultRenderTarget();

	protected:

	private:
		void LogAdapterOutputs(IDXGIAdapter* adapter);
		void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

		Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState;
		

		static const int SwapChainBufferCount = 1;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_SwapChainBuffer[SwapChainBufferCount];

		UINT64 m_CurrentFence = 0;
		UINT m_RtvDescriptorSize = 0;
		UINT m_DsvDescriptorSize = 0;
		UINT m_CbvSrvUavDescriptorSize = 0;

		DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Set true to use 4X MSAA (§4.1.8).  The default is false.
		bool      m_4xMsaaState = false; // 4X MSAA enabled
		UINT      m_4xMsaaQuality = 0;      // quality level of 4X MSAA

		int mCurrBackBuffer = 0;
		HWND m_hMainWnd;
		int m_ClientWidth = 800;
		int m_ClientHeight = 600;

		D3D11_VIEWPORT m_ScreenViewport;
		D3D11_RECT m_ScissorRect;

		std::vector<IDXGIAdapter*> adapterList;
		DirectX::XMFLOAT4 clearColor;

		Camera* mainCamera = nullptr;
		DirectX::BoundingFrustum frameFrustrum;

		bool frustrumCullingEnabled = true;
		FrameStats frameStats;
		FrameBuffer framebufferForTexture;

	};



}

