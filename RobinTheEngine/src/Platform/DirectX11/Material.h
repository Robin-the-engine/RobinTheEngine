#pragma once
#include <DirectXMath.h>
#include "Platform/DirectX11/Shaders.h"
#include "Platform/DirectX11/ConstantBuffer.h"

namespace RTE {

	class Material {

	public:
		Material();
		virtual void ApplyMaterial();
		virtual ~Material();

	};


	//Default material for all objects without materials.
	//We just draw objects with gray collor.
	class DefaultMaterial : public Material 
	{

	public:
		DefaultMaterial();
		void SetColor(DirectX::XMFLOAT4 colorVec);

		// Inherited via Material
		virtual void ApplyMaterial() override;

	private:
		vertexShader vs;
		pixelShader ps;
		ConstantBuffer<CB_VS_VEC4F> color;
		
	};




}
