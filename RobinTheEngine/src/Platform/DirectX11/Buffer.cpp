#pragma once
#include "rtepch.h"
#include "Buffer.h"
#include "RobinTheEngine/ComsManager.h"
#include <d3d11.h>
#include "RobinTheEngine/ResourceFactory.h"


RTE::IndexBuffer::IndexBuffer(DWORD* data, int arraySize, std::string bufferName)
{
	using namespace D3DUtils;


	elementCount = arraySize;

	name = bufferName;

	if (ComsManager::Get().IsHaveComPtrResource(name+ "\\IndexBuffer")) {
		auto res = ComsManager::Get().GetComPtrResource<ID3D11Buffer>(name + "\\IndexBuffer");
		return;

	}
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

	ThrowIfFailed(GetDevice()->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf()));
	ComsManager::Get().RegisterComPtrResource<ID3D11Buffer>(buffer, name + "\\IndexBuffer");

}

RTE::IndexBuffer::~IndexBuffer()
{
}

HRESULT RTE::IndexBuffer::Init(DWORD * data, int arraySize, std::string bufferName)
{
	elementCount = arraySize;
	name = bufferName;


	if (ComsManager::Get().IsHaveComPtrResource(name + "\\IndexBuffer")) {
		auto res = ComsManager::Get().GetComPtrResource<ID3D11Buffer>(name + "\\IndexBuffer");
		return S_OK;

	}


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

	auto res = GetDevice()->CreateBuffer(&desk, &bufferData, buffer.GetAddressOf());
	if(SUCCEEDED(res))
	ComsManager::Get().RegisterComPtrResource<ID3D11Buffer>(buffer, name + "\\IndexBuffer");
	return res;
}
