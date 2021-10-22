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
		Mesh(std::vector<vertex_Gouraud_shading> & vertices, std::vector<DWORD> & indices, std::vector<Texture>& textures, std::string meshName);
		Mesh(const Mesh & mesh);
		void Draw();
		std::vector<Texture> textures;
	private:
		vertexBuffer<vertex_Gouraud_shading> vertexbuffer;
		IndexBuffer indexbuffer;
		ID3D11DeviceContext * deviceContext;
		std::string name;

	};




}








