#include "rtepch.h"
#include "Shaders.h"
#include "D3DCompiler.h"
#include "RobinTheEngine/Log.h"
#include "RobinTheEngine/Application.h"
#include "DirectX11RenderSystem.h"

RTE::vertexShader::vertexShader(std::wstring filePath) :path(filePath)
{

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
			char*  compileErrors = (char*)(errorMessage->GetBufferPointer());
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

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		D3D11_INPUT_ELEMENT_DESC {"POSITION",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		D3D11_INPUT_ELEMENT_DESC {"COLOR", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);
	ThrowIfFailed(rs->GetDevice()->CreateInputLayout(layout, numElements, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), inputLayout.GetAddressOf()));

}

RTE::vertexShader::~vertexShader()
{
	/*shader->Release();
	shaderBuffer->Release();*/
}

ID3D11VertexShader * RTE::vertexShader::GetShader()
{
	return shader.Get();
}

ID3DBlob * RTE::vertexShader::GetBuffer()
{
	return shaderBuffer.Get();
}

ID3D11InputLayout* RTE::vertexShader::GetInputLayout()
{
	return inputLayout.Get();
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
			char*  compileErrors = (char*)(errorMessage->GetBufferPointer());
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
	/*shader->Release();
	shaderBuffer->Release();*/
}

ID3D11PixelShader * RTE::pixelShader::GetShader()
{
	return shader.Get();
}

ID3DBlob * RTE::pixelShader::GetBuffer()
{
	return shaderBuffer.Get();
}
