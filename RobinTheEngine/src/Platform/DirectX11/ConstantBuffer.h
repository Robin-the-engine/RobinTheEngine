#pragma once
#include "d3d11.h"
#include <wrl.h>
#include <DirectXMath.h> 
#include "RobinTheEngine/d3dUtils.h"
#include "RobinTheEngine/ComsManager.h"

#include "Platform/DirectX11/LightStructure.h"

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

	struct CB_PS_NUM_LIGHTS
	{
		int numLights;
		int foo;
		int bar;
		int foobar;
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

	struct CB_PS_LIGHTS
	{
		DirectX::XMFLOAT3 padding;
		int numLights;

		Light light;
	};


	struct CB_PS_MATERIAL
	{

		XMFLOAT4  AmbientColor;
		XMFLOAT4  EmissiveColor;
		XMFLOAT4  DiffuseColor;
		XMFLOAT4  SpecularColor;
		XMFLOAT4  Reflectance;
		float   Opacity;
		float   SpecularPower;
		float   IndexOfRefraction;

		uint32_t HasAmbientTexture;
		uint32_t HasEmissiveTexture;
		uint32_t HasDiffuseTexture;
		uint32_t HasSpecularTexture;
		uint32_t HasSpecularPowerTexture;
		uint32_t HasNormalTexture;
		uint32_t HasBumpTexture;
		uint32_t HasOpacityTexture;

		float   BumpIntensity;
		float   SpecularScale;
		float   AlphaThreshold;
		float padding[2];

		CB_PS_MATERIAL()
			: AmbientColor(0, 0, 0, 1)
			, EmissiveColor(0, 0, 0, 1)
			, DiffuseColor(1, 1, 1, 1)
			, SpecularColor(0, 0, 0, 1)
			, Reflectance(0, 0, 0, 0)
			, Opacity(1.0f)
			, SpecularPower(-1.0f)
			, IndexOfRefraction(-1.0f)
			, HasAmbientTexture(false)
			, HasEmissiveTexture(false)
			, HasDiffuseTexture(false)
			, HasSpecularTexture(false)
			, HasSpecularPowerTexture(false)
			, HasNormalTexture(false)
			, HasBumpTexture(false)
			, HasOpacityTexture(false)
			, BumpIntensity(5.0f)
			, SpecularScale(128.0f)
			, AlphaThreshold(0.1f)
		{}

	};


}


