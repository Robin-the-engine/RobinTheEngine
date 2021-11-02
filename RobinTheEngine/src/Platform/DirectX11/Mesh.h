#pragma once

#include "Platform/DirectX11/Vertex.h"
#include "Platform/DirectX11/Buffer.h"
#include "Platform/DirectX11/ConstantBuffer.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Platform/DirectX11/Texture.h"


namespace RTE {

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

	};







	class IMeshTwo {

	public:
		//Bind mesh to rendering pipeline
		virtual void BindMesh( ID3D11DeviceContext* context) {
			RTE_CORE_ASSERT(false, "we dont use that implementation");
		}


	};


	template<class T>
	class MeshTwo : public IMeshTwo
	{
	public:
		MeshTwo(std::vector<T>& vertices, std::vector<DWORD>& indices){

			ThrowIfFailed(vert.Init(vertices.data(), vertices.size(), "name"));
			ThrowIfFailed(indexBuffer.Init(indices.data(), indices.size(), "name"));
		}
		MeshTwo(const MeshTwo& mesh) = default;


		void BindMesh(ID3D11DeviceContext* context) override
		{
			UINT offset = 0;
			//context->IASetVertexBuffers(0, 1, vertexBuff.GetAddressOf(), vertexBuff.StridePtr(), &offset);
			//context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
			
		}
		RTE::vertexBuffer<T>vert;
		//vertexBuffer<T> vertexBuff;
		IndexBuffer indexBuffer;
	
	};



}








