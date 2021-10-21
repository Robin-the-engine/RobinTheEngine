#pragma once
#include "d3d11.h"
#include <wrl.h>
#include "Platform/DirectX11/DirectX11RenderSystem.h"
#include "RobinTheEngine/Application.h"

namespace RTE {

	template<class T>
	class vertexBuffer {

	public:
		vertexBuffer() {};
		vertexBuffer(T* data, int numOfElements) {
			//TODO: reuse init function for construction
			DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			this->bufferSize = numOfElements * sizeof(T);
			stride = sizeof(T);

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
		}
		HRESULT Init(T* data, int numOfElements) {
			//TODO: check is buffer initialized 
			DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			this->bufferSize = numOfElements * sizeof(T);
			stride = sizeof(T);

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

			return rs->GetDevice()->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf());
		}
		vertexBuffer(const vertexBuffer<T>& rhs)
		{
			this->buffer = rhs.buffer;
			this->bufferSize = rhs.bufferSize;
			this->stride = rhs.stride;
		}
		vertexBuffer<T> & operator=(const vertexBuffer<T>& a)
		{
			this->buffer = a.buffer;
			this->bufferSize = a.bufferSize;
			this->stride = a.stride;
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

	};

	class IndexBuffer {
	public:
		IndexBuffer(DWORD* data, int arraySize);
		IndexBuffer() {};
		~IndexBuffer();
		HRESULT Init(DWORD* data, int arraySize);



	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		UINT elementCount;
	
	public:
		ID3D11Buffer* Get() { return buffer.Get(); }
		ID3D11Buffer* const* GetAdressOf() { return buffer.GetAddressOf(); }
		UINT ElementCount() { return elementCount; }

	};
}
