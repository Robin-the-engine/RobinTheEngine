#include "rtepch.h"
#include "Platform/DirectX11/ConstantBuffer.h"

/*
template<>
RTE::ConstantBuffer<CB_VS_MATRIX4x4>::ConstantBuffer(std::string bufferName) : name(bufferName)
		{

			if (ResourceManager::Get().IsHaveComPtrResource(name + "\\ConstBuffer")) {
				buffer = ResourceManager::Get().GetComPtrResource<ID3D11Buffer>(name + "\\ConstBuffer");
				return;
			}

			DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

			D3D11_BUFFER_DESC desk;
			ZeroMemory(&desk, sizeof(desk));

			desk.Usage = D3D11_USAGE_DYNAMIC;
			desk.ByteWidth = sizeof(CB_VS_MATRIX4x4) + (16 - (sizeof(CB_VS_MATRIX4x4) % 16));
			desk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desk.MiscFlags = 0;
			desk.StructureByteStride = 0; //?
 
			ThrowIfFailed(rs->GetDevice()->CreateBuffer(&desk, 0, buffer.GetAddressOf()));

			ResourceManager::Get().RegisterComPtrResource(buffer, name + "\\ConstBuffer");
		}*/

