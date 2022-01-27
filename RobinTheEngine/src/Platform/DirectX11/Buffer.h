#pragma once
#include "d3d11.h"
#include <wrl.h>
#include "RobinTheEngine/ComsManager.h"


namespace RTE {

	class Aplication;
	class ComsManager;

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();

	template<class T>
	class vertexBuffer {

	public:
		vertexBuffer() {};
		vertexBuffer(T* data, int numOfElements, std::string bufferName) {
			using namespace D3DUtils;
			//TODO: reuse init function for construction
			auto device = GetDevice();
			//DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			this->bufferSize = numOfElements * sizeof(T);
			stride = sizeof(T);
			name = bufferName;

			if (ComsManager::Get().IsHaveComPtrResource(name + "\\VertexBuffer")) {
				auto res = ComsManager::Get().GetComPtrResource<ID3D11Buffer>(name + "\\VertexBuffer");
				return;

			}


			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));

			desk.Usage = D3D11_USAGE_DEFAULT;
			desk.ByteWidth = sizeof(T) * numOfElements;
			desk.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desk.CPUAccessFlags = 0;
			desk.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(bufferData));
			bufferData.pSysMem = data;

			ThrowIfFailed(device->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf()));

			ComsManager::Get().RegisterComPtrResource<ID3D11Buffer>(buffer, name + "\\VertexBuffer");
		}
		HRESULT Init(T* data, int numOfElements, std::string bufferName) {
			//TODO: check is buffer initialized 
			//DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
			auto device = GetDevice();
			this->bufferSize = numOfElements * sizeof(T);
			stride = sizeof(T);
			name = bufferName;

			if (ComsManager::Get().IsHaveComPtrResource(name + "\\VertexBuffer")) {
				buffer = ComsManager::Get().GetComPtrResource<ID3D11Buffer>(name + "\\VertexBuffer");
				return S_OK;

			}

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));

			desk.Usage = D3D11_USAGE_DEFAULT;
			desk.ByteWidth = sizeof(T) * numOfElements;
			desk.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desk.CPUAccessFlags = 0;
			desk.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(bufferData));
			bufferData.pSysMem = data;

			auto result = device->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf());
			if (SUCCEEDED(result))
				ComsManager::Get().RegisterComPtrResource<ID3D11Buffer>(buffer, name + "\\VertexBuffer");

			return result;
		}
		vertexBuffer(const vertexBuffer<T>& rhs)
		{
			this->buffer = rhs.buffer;
			this->bufferSize = rhs.bufferSize;
			this->stride = rhs.stride;
			this->name = rhs.name;
		}
		vertexBuffer<T>& operator=(const vertexBuffer<T>& a)
		{
			this->buffer = a.buffer;
			this->bufferSize = a.bufferSize;
			this->stride = a.stride;
			this->name = a.name;
			return *this;
		}

		~vertexBuffer() {};

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT stride;
		UINT bufferSize;

		vertexBuffer(vertexBuffer& rhs) = delete;
		vertexBuffer(vertexBuffer&& rhs) = delete;

	public:
		ID3D11Buffer* Get() { return buffer.Get(); }
		ID3D11Buffer* const* GetAddressOf() { return buffer.GetAddressOf(); }
		UINT BufferSize() { return bufferSize; }
		UINT* StridePtr() { return &stride; }
		UINT Stride() { return stride; }
		std::string name;

	};

	class IndexBuffer {
	public:
		IndexBuffer(DWORD* data, int arraySize, std::string name);
		IndexBuffer() {};
		~IndexBuffer();
		HRESULT Init(DWORD* data, int arraySize, std::string name);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT elementCount;
		std::string name;

	public:
		ID3D11Buffer* Get() { return buffer.Get(); }
		ID3D11Buffer* const* GetAdressOf() { return buffer.GetAddressOf(); }
		UINT ElementCount() { return elementCount; }

	};



	template<class T>
	class structuredBuffer {

	public:
		structuredBuffer() { bufferSize = 0; };
		structuredBuffer(T* data, int numOfElements) {
			using namespace D3DUtils;
			auto device = GetDevice();

			this->bufferSize = numOfElements * sizeof(T);
			stride = sizeof(T);

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));

			desk.Usage = D3D11_USAGE_DYNAMIC;
			desk.ByteWidth = sizeof(T) * numOfElements;
			desk.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desk.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desk.StructureByteStride = stride;

			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(bufferData));
			bufferData.pSysMem = data;

			ThrowIfFailed(device->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf()));

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.FirstElement = 0;
			srvDesc.Buffer.NumElements = numOfElements;

			ThrowIfFailed(device->CreateShaderResourceView(buffer.Get(), &srvDesc, m_pSRV.GetAddressOf()));
			isInited = true;


		}

		void Init(T* data, int numOfElements) {

			using namespace D3DUtils;
			auto device = GetDevice();
			RTE_CORE_ASSERT(isInited, "That buffer already initialized!");

			this->bufferSize = numOfElements * sizeof(T);
			stride = sizeof(T);

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));

			desk.Usage = D3D11_USAGE_DYNAMIC;
			desk.ByteWidth = sizeof(T) * numOfElements;
			desk.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desk.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desk.StructureByteStride = stride;

			D3D11_SUBRESOURCE_DATA bufferData;
			ZeroMemory(&bufferData, sizeof(bufferData));
			bufferData.pSysMem = data;


			ThrowIfFailed(device->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf()));

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.FirstElement = 0;
			srvDesc.Buffer.NumElements = numOfElements;

			ThrowIfFailed(device->CreateShaderResourceView(buffer.Get(), &srvDesc, m_pSRV.GetAddressOf()));
			isInited = true;
		}

		void Resize(T* data, int numOfElements) {

			buffer.Reset();
			m_pSRV.Reset();
			isInited = false;
			Init(data, numOfElements);
		}

		void Update(T* data, int numOfElements) {

			using namespace D3DUtils;
			auto context = GetContext();
			ID3D11ShaderResourceView* nullSRV[] = {NULL};
			context->PSSetShaderResources(0, 1, nullSRV);

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			// Copy the contents of the data buffer to the GPU.

			ThrowIfFailed(context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		
			size_t sizeInBytes = numOfElements * sizeof(T);
			memcpy_s(mappedResource.pData, sizeInBytes, data, sizeInBytes);
			
			context->Unmap(buffer.Get(), 0);

		}

		structuredBuffer(const structuredBuffer<T>& rhs)
		{
			this->buffer = rhs.buffer;
			this->bufferSize = rhs.bufferSize;
			this->stride = rhs.stride;
			this->name = rhs.name;
		}

		structuredBuffer<T>& operator=(const structuredBuffer<T>& a)
		{
			this->buffer = a.buffer;
			this->bufferSize = a.bufferSize;
			this->stride = a.stride;
			this->name = a.name;
			return *this;
		}

		~structuredBuffer() {};

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pUAV;

		UINT stride;
		UINT bufferSize;

		structuredBuffer(structuredBuffer& rhs) = delete;
		structuredBuffer(structuredBuffer&& rhs) = delete;

	public:
		ID3D11Buffer* Get() { return buffer.Get(); }
		ID3D11Buffer* const* GetAddressOf() { return buffer.GetAddressOf(); }
		ID3D11ShaderResourceView* GetSRV() { return m_pSRV.GetAddressOf(); }
		ID3D11ShaderResourceView* const* GetAddressOfSRV() { return m_pSRV.GetAddressOf(); }
		UINT BufferSize() { return bufferSize; }
		UINT* StridePtr() { return &stride; }
		UINT Stride() { return stride; }
		std::string name;
		bool isInited;

	};





}
