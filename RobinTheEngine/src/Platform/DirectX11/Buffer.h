#pragma once
#include "d3d11.h"
#include <wrl.h>
#include "Platform/DirectX11/DirectX11RenderSystem.h"
#include "RobinTheEngine/Application.h"
#include "RobinTheEngine/ComsManager.h"


namespace RTE {

	class Aplication;
	class ComsManager;

	template<class T>
	class vertexBuffer {

	public:
		vertexBuffer() {};
		vertexBuffer(T* data, int numOfElements, std::string bufferName) {
			using namespace D3DUtils;
			//TODO: reuse init function for construction
			DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

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

			ThrowIfFailed(rs->GetDevice()->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf()));

			ComsManager::Get().RegisterComPtrResource<ID3D11Buffer>(buffer, name + "\\VertexBuffer");
		}
		HRESULT Init(T* data, int numOfElements, std::string bufferName) {
			//TODO: check is buffer initialized 
			DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			this->bufferSize = numOfElements * sizeof(T);
			stride = sizeof(T);
			name = bufferName;

			if (ComsManager::Get().IsHaveComPtrResource(name + "\\VertexBuffer")) {
				auto res = ComsManager::Get().GetComPtrResource<ID3D11Buffer>(name + "\\VertexBuffer");
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

			auto result =  rs->GetDevice()->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf());
			if(SUCCEEDED(result))
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
		vertexBuffer<T> & operator=(const vertexBuffer<T>& a)
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
		UINT * StridePtr() { return &stride; }
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
}
