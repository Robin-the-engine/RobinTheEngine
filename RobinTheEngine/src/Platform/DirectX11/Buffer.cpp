#pragma once
#include "rtepch.h"
#include "Buffer.h"
#include "DirectX11RenderSystem.h"
#include "RobinTheEngine/Application.h"



RTE::IndexBuffer::IndexBuffer(DWORD* data, int arraySize)
{
	DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

	elementCount = arraySize;

	D3D11_BUFFER_DESC desk;
	ZeroMemory(&desk, sizeof(desk));

	desk.Usage = D3D11_USAGE_DEFAULT;
	desk.ByteWidth = sizeof(DWORD) * elementCount;
	desk.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desk.CPUAccessFlags = 0;
	desk.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA bufferData;
	ZeroMemory(&bufferData, sizeof(bufferData));
	bufferData.pSysMem = data;

	ThrowIfFailed(rs->GetDevice()->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf()));

}

RTE::IndexBuffer::~IndexBuffer()
{
}

HRESULT RTE::IndexBuffer::Init(DWORD * data, int arraySize)
{
	DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());

	elementCount = arraySize;

	D3D11_BUFFER_DESC desk;
	ZeroMemory(&desk, sizeof(desk));

	desk.Usage = D3D11_USAGE_DEFAULT;
	desk.ByteWidth = sizeof(DWORD) * elementCount;
	desk.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desk.CPUAccessFlags = 0;
	desk.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA bufferData;
	ZeroMemory(&bufferData, sizeof(bufferData));
	bufferData.pSysMem = data;

	return rs->GetDevice()->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf());
}
