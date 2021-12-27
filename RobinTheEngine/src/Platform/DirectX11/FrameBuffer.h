#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>

namespace RTE {


	class FrameBuffer
	{
	public:

		void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, int textureWidth, int textureHeight);
		void SetRenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void ClearRenderTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,  DirectX::XMFLOAT4 color);
		void Resize(Microsoft::WRL::ComPtr<ID3D11Device> device, int textureWidth, int textureHeight);
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView();

		int Width = -1;
		int Height = -1;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTargetTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
		bool initializedFlag = false;
	};



}


