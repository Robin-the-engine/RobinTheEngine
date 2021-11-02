#pragma once
#include "d3d11.h"
#include <wrl.h>
#include <string>


namespace RTE {


	class vertexShader
	{
	public:
		vertexShader(std::wstring filePath, const D3D11_INPUT_ELEMENT_DESC layoutElements[], UINT numLayoutElements );
		~vertexShader();

		ID3D11VertexShader* GetShader();
		ID3DBlob* GetBuffer();
		ID3D11InputLayout* GetInputLayout();

	private:
		std::wstring path;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	};

	class pixelShader
	{
	public:
		pixelShader(std::wstring filePath);
		~pixelShader();

		ID3D11PixelShader* GetShader();
		ID3DBlob* GetBuffer();

	private:
		std::wstring path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;

	};







}
