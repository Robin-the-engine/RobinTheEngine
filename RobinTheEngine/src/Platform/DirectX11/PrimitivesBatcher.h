#pragma once
#include "TKHeaders.h"
#include <memory>
#include "RobinTheEngine/Core.h"




namespace RTE {


	class PrimitivesBatcher {
	public:
		static void Init(ID3D11Device* device, ID3D11DeviceContext* context, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection);
		static void SetViewProjection(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection);
		static void DrawPrimitive(ID3D11DeviceContext* context, BoundingFrustum frustum);
		static void DrawPrimitive(ID3D11DeviceContext* context, BoundingOrientedBox orientedBox);
		static void DrawPrimitive(ID3D11DeviceContext* context, BoundingSphere sphere);
		static void DrawPrimitive(ID3D11DeviceContext* context, BoundingBox box);
		static void DrawGrid(ID3D11DeviceContext* context);

	private:



	};



}
