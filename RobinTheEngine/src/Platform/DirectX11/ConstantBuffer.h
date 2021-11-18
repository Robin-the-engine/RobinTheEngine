#pragma once
#include "d3d11.h"
#include <wrl.h>
#include <DirectXMath.h> 
#include "RobinTheEngine/d3dUtils.h"
#include "RobinTheEngine/ComsManager.h"

namespace RTE {

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	class Application;

	template<class T>
	class  ConstantBuffer {

	public:
		T data;

		void InitializeUniqueBuffer() {
			using namespace D3DUtils;

			auto device = GetDevice();

			//DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(RTE::Application::Get().GetRenderSystem());

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));
			desk.Usage = D3D11_USAGE_DYNAMIC;
			desk.ByteWidth = sizeof(T) + (16 - (sizeof(T) % 16));
			desk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desk.MiscFlags = 0;
			desk.StructureByteStride = 0; //?

			ThrowIfFailed(device->CreateBuffer(&desk, 0, buffer.GetAddressOf()));
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
			auto device = GetDevice();
			//DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));

			desk.Usage = D3D11_USAGE_DYNAMIC;
			desk.ByteWidth = sizeof(T) + (16 - (sizeof(T) % 16));
			desk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desk.MiscFlags = 0;
			desk.StructureByteStride = 0; //?

			ThrowIfFailed(device->CreateBuffer(&desk, 0, buffer.GetAddressOf()));

			ComsManager::Get().RegisterComPtrResource(buffer, name + "\\ConstBuffer");

		}

		ConstantBuffer() : isInitialized(false)
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
			auto context = GetContext();
			//RTE::DirectX11RenderSystem* rs = static_cast<RTE::DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
			D3D11_MAPPED_SUBRESOURCE subres;
			ThrowIfFailed(context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subres));
			CopyMemory(subres.pData, &data, sizeof(T));
			context->Unmap(buffer.Get(), 0);
		}
		void WirteBuffer(T data) {

			this->data = data;
			WriteBuffer();
		}

	};



	struct CB_VS_CAMERA
	{
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;

	};

	struct CB_VS_WORLD_MAT
	{
		DirectX::XMFLOAT4X4 worldMatrix;
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


