#include "rtepch.h"
#include "Shaders.h"
#include "D3DCompiler.h"
#include "RobinTheEngine/Log.h"
#include "RobinTheEngine/Application.h"
#include "DirectX11RenderSystem.h"
#include "RobinTheEngine/ComsManager.h"

using namespace D3DUtils;

RTE::vertexShader::vertexShader(std::wstring filePath, const D3D11_INPUT_ELEMENT_DESC layoutElements[], UINT numLayoutElements) :path(filePath)
{
	std::string charPath(path.begin(), path.end());
	if (ComsManager::Get().IsHaveComPtrResource(charPath + "\\VertexShader")) {
		shader = ComsManager::Get().GetComPtrResource<ID3D11VertexShader>(charPath + "\\VertexShader");
		inputLayout = ComsManager::Get().GetComPtrResource<ID3D11InputLayout>(charPath + "\\InputLayout");
		return;

	}

	Microsoft::WRL::ComPtr<ID3DBlob> errorMessage;
	HRESULT hr = D3DCompileFromFile(path.c_str(), //path
		NULL, //macro
		NULL,//include
		"main",//entrypoint
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderBuffer.GetAddressOf(),
		errorMessage.GetAddressOf());

	if (FAILED(hr)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage) {
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			RTE_WARN(compileErrors);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			RTE_CORE_WARN("Missing Shader File");
		}
		ThrowIfFailed(hr);
	}

	DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	ThrowIfFailed(rs->GetDevice()->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, shader.GetAddressOf()));

	/*
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		D3D11_INPUT_ELEMENT_DESC {"TEXCOORD",0,DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		D3D11_INPUT_ELEMENT_DESC {"NORMAL", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC {"POSITION", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	*/
	//UINT numElements = ARRAYSIZE(layout);
//	UINT numElements = ARRAYSIZE(layoutElements);
	ThrowIfFailed(rs->GetDevice()->CreateInputLayout(layoutElements, numLayoutElements, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), inputLayout.GetAddressOf()));

	{
		ComsManager::Get().RegisterComPtrResource<ID3D11VertexShader>(shader, charPath + "\\VertexShader");
		ComsManager::Get().RegisterComPtrResource<ID3D11InputLayout>(inputLayout, charPath + "\\InputLayout");
	}

}

RTE::vertexShader::~vertexShader()
{
}

ID3D11VertexShader* RTE::vertexShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* RTE::vertexShader::GetBuffer()
{
	return shaderBuffer.Get();
}

ID3D11InputLayout* RTE::vertexShader::GetInputLayout()
{
	return inputLayout.Get();
}

void RTE::vertexShader::init(std::string filePath, const D3D11_INPUT_ELEMENT_DESC layoutElements[], UINT numLayoutElements)
{
	std::string charPath = filePath;
	std::wstring wcharpath(filePath.begin(), filePath.end());
	path = wcharpath;
	
	if (ComsManager::Get().IsHaveComPtrResource(charPath + "\\VertexShader")) {
		shader = ComsManager::Get().GetComPtrResource<ID3D11VertexShader>(charPath + "\\VertexShader");
		inputLayout = ComsManager::Get().GetComPtrResource<ID3D11InputLayout>(charPath + "\\InputLayout");
		return;

	}

	Microsoft::WRL::ComPtr<ID3DBlob> errorMessage;
	HRESULT hr = D3DCompileFromFile(path.c_str(), //path
		NULL, //macro
		NULL,//include
		"main",//entrypoint
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderBuffer.GetAddressOf(),
		errorMessage.GetAddressOf());

	if (FAILED(hr)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage) {
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			RTE_WARN(compileErrors);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			RTE_CORE_WARN("Missing Shader File");
		}
		ThrowIfFailed(hr);
	}

	DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	ThrowIfFailed(rs->GetDevice()->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, shader.GetAddressOf()));

	ThrowIfFailed(rs->GetDevice()->CreateInputLayout(layoutElements, numLayoutElements, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), inputLayout.GetAddressOf()));
	{
		ComsManager::Get().RegisterComPtrResource<ID3D11VertexShader>(shader, charPath + "\\VertexShader");
		ComsManager::Get().RegisterComPtrResource<ID3D11InputLayout>(inputLayout, charPath + "\\InputLayout");
	}

}

RTE::pixelShader::pixelShader(std::wstring filePath) :path(filePath)
{
	Microsoft::WRL::ComPtr<ID3DBlob> errorMessage;
	HRESULT hr = D3DCompileFromFile(path.c_str(), //path
		NULL, //macro
		NULL,//include
		"main",//entrypoint
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderBuffer.GetAddressOf(),
		errorMessage.GetAddressOf());

	if (FAILED(hr)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage) {
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			RTE_WARN(compileErrors);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			RTE_CORE_WARN("Missing Shader File");
		}
		ThrowIfFailed(hr);
	}

	DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	ThrowIfFailed(rs->GetDevice()->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, shader.GetAddressOf()));
}

RTE::pixelShader::~pixelShader()
{
}

ID3D11PixelShader* RTE::pixelShader::GetShader()
{
	return shader.Get();
}

ID3DBlob* RTE::pixelShader::GetBuffer()
{
	return shaderBuffer.Get();
}


void RTE::pixelShader::init(std::wstring filepath)
{
	path = filepath;
	std::string charPath(filepath.begin(), filepath.end());

	if (ComsManager::Get().IsHaveComPtrResource(charPath + "\\PixelShader")) {
		shader = ComsManager::Get().GetComPtrResource<ID3D11PixelShader>(charPath + "\\PixelShader");
		return;
	}

	Microsoft::WRL::ComPtr<ID3DBlob> errorMessage;
	HRESULT hr = D3DCompileFromFile(path.c_str(), //path
		NULL, //macro
		NULL,//include
		"main",//entrypoint
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderBuffer.GetAddressOf(),
		errorMessage.GetAddressOf());

	if (FAILED(hr)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorMessage) {
			char* compileErrors = (char*)(errorMessage->GetBufferPointer());
			RTE_WARN(compileErrors);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			RTE_CORE_WARN("Missing Shader File");
		}
		ThrowIfFailed(hr);
	}

	DirectX11RenderSystem* rs = static_cast<DirectX11RenderSystem*>(Application::Get().GetRenderSystem());
	ThrowIfFailed(rs->GetDevice()->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, shader.GetAddressOf()));

	{
		ComsManager::Get().RegisterComPtrResource<ID3D11PixelShader>(shader, charPath + "\\PixelShader");
	}

}

