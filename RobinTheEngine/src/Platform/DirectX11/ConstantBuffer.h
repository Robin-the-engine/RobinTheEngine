#pragma once
#include "d3d11.h"
#include <wrl.h>
#include <DirectXMath.h> 
#include "RobinTheEngine/d3dUtils.h"
#include "RobinTheEngine/Application.h"
#include "RobinTheEngine/ComsManager.h"
#include "Platform/DirectX11/DirectX11RenderSystem.h"
#include "RobinTheEngine/d3dUtils.h"

namespace RTE {

	class Application;

	template<class T>
	class  ConstantBuffer {

	public:
		T data;

		void InitializeUniqueBuffer() {
			using namespace D3DUtils;
			

			DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));
			desk.Usage = D3D11_USAGE_DYNAMIC;
			desk.ByteWidth = sizeof(T) + (16 - (sizeof(T) % 16));
			desk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desk.MiscFlags = 0;
			desk.StructureByteStride = 0; //?

			ThrowIfFailed(rs->GetDevice()->CreateBuffer(&desk, 0, buffer.GetAddressOf()));
			isInitialized = true;

		}

		void InitializeSharedBuffer(std::string bufferName) {
			this->name = bufferName;
			isInitialized = true;
			using namespace D3DUtils;
			if (ComsManager::Get().IsHaveComPtrResource(name + "\\ConstBuffer")) {
				buffer = ComsManager::Get().GetComPtrResource<ID3D11Buffer>(name + "\\ConstBuffer");
				return;
			}

			DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));

			desk.Usage = D3D11_USAGE_DYNAMIC;
			desk.ByteWidth = sizeof(T) + (16 - (sizeof(T) % 16));
			desk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desk.MiscFlags = 0;
			desk.StructureByteStride = 0; //?

			ThrowIfFailed(rs->GetDevice()->CreateBuffer(&desk, 0, buffer.GetAddressOf()));

			ComsManager::Get().RegisterComPtrResource(buffer, name + "\\ConstBuffer");

		}

		ConstantBuffer(): isInitialized(false)
		{
		}



	private:
		std::string name;
		bool isInitialized;
		int slot;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		//ConstantBuffer(ConstantBuffer<T>& rhs) = delete;
		//ConstantBuffer(ConstantBuffer<T>&& rhs) = delete;
		//ConstantBuffer& operator=(ConstantBuffer<T>& rhs) = delete;
		//ConstantBuffer& operator=(ConstantBuffer<T>&& rhs) = delete;

	public:
		ID3D11Buffer* Get() { return buffer.Get(); }
		ID3D11Buffer* const* GetAddressOf() { return buffer.GetAddressOf(); }
		void WriteBuffer() {
			using namespace D3DUtils;
			RTE_CORE_ASSERT(isInitialized, "That buffer is not initialized!");
			RTE::DirectX11RenderSystem* rs = static_cast<RTE::DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
			D3D11_MAPPED_SUBRESOURCE subres;
			ThrowIfFailed(rs->GetContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subres));
			CopyMemory(subres.pData, &data, sizeof(T));
			rs->GetContext()->Unmap(buffer.Get(), 0);
		}
		void WirteBuffer(T data) {

			this->data = data;
			WriteBuffer();
		}

	};





		//Constant buffer types
	struct CB_VS_MATRIX4x4
	{
		DirectX::XMFLOAT4X4 mvpMatrix;
		DirectX::XMFLOAT4X4 worldMatrix;
	};

	struct CB_VS_VEC4F
	{
		DirectX::XMFLOAT4 vec;
	};

	struct CB_PS_LIGHT
	{
		DirectX::XMFLOAT3 ambientLightColor;
		float ambientStrength;

		DirectX::XMFLOAT3 diffuseCollor;
		float diffuseStrenght;

		DirectX::XMFLOAT3 lightPosition;
		float padding;

		DirectX::XMFLOAT3 viewPosition;
		float specularStrength;
	};

}


