#pragma once

#include "Platform/DirectX11/Vertex.h"
#include "Platform/DirectX11/Buffer.h"
//#include "Platform/DirectX11/ConstantBuffer.h"

//#include "Platform/DirectX11/Texture.h"

//#include "RobinTheEngine/Scene/BaseResource.h"


namespace RTE {

/*
	//  TEMP: Abstract Mesh class
	class IMesh : public BaseResource {

	public:

		virtual void BindMesh(ID3D11DeviceContext* context) {
			RTE_CORE_ASSERT(false, "we dont use that implementation");
		}

		int elementCount;

		IMesh(ResourceID id) : BaseResource(id) { }
		IMesh() : BaseResource("invalid") {};
		virtual void Draw(const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewProjectionMatrix) { RTE_CORE_ASSERT("That") };
	};*/

	class IMesh {

	public:
		//Bind mesh to rendering pipeline
		virtual void BindMesh(ID3D11DeviceContext* context) {
			RTE_CORE_ASSERT(false, "we dont use that implementation");
		}
		int elementCount;
		DirectX::XMFLOAT3 min = DirectX::XMFLOAT3(0, 0, 0);
		DirectX::XMFLOAT3 max = DirectX::XMFLOAT3(0, 0, 0);

	};


	template<class T>
	class Mesh : public IMesh
	{
	public:
		Mesh(std::vector<T>& vertices, std::vector<DWORD>& indices, std::string id) {
			using namespace D3DUtils;

			ThrowIfFailed(vert.Init(vertices.data(), vertices.size(), id));
			ThrowIfFailed(indexBuffer.Init(indices.data(), indices.size(), id));
			elementCount = indexBuffer.ElementCount();
		}
		Mesh(const Mesh& mesh) = default;


		void BindMesh(ID3D11DeviceContext* context) override
		{
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, vert.GetAddressOf(), vert.StridePtr(), &offset);
			context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

		}
		//TODO: rename vertex buffer and rename class name 
		RTE::vertexBuffer<T>vert;
		IndexBuffer indexBuffer;

	};











	/*
	class Mesh
	{
	public:
		Mesh(std::vector<vertex_Gouraud_shading>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, std::string meshName);
		Mesh(const Mesh& mesh);
		void Draw();
		std::vector<Texture> textures;
	private:
		vertexBuffer<vertex_Gouraud_shading> vertexbuffer;
		IndexBuffer indexbuffer;
		ID3D11DeviceContext* deviceContext;
		std::string name;

	};*/


}








