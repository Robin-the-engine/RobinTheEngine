#pragma once
#include "RobinTheEngine/Scene/Component.h"
#include "Platform/DirectX11/Mesh.h"

namespace RTE
{
	// TEMP: Remove when the real Material class is added;
	struct Material
	{
		bool some_data;
	};


	struct MeshRenderer: public Component
	{
	private:
		Mesh mesh;
		Material material;

	public:
		MeshRenderer(const MeshRenderer&) = default;
		MeshRenderer(const Mesh& mesh, const Material& material)
			: mesh(mesh), material(material) { }

		Mesh GetMesh() const { return mesh; }
		Material GetMaterial() const { return material; }

		void SetMesh(const Mesh& newMesh) { mesh = newMesh; }
		Material SetMaterial(const Material& newMaterial) { material = newMaterial; }
	};
}