#pragma once
#include "RobinTheEngine/Scene/Component.h"
#include <DirectXMath.h>
#include "RobinTheEngine/Scene/BaseResource.h"
#include "RobinTheEngine/Scene/Components/IDrawable.h"

#include "Platform/DirectX11/Model.h"
#include "Platform/DirectX11/Material.h"


namespace RTE
{
	struct MeshRenderer : public Component
	{
	private:
		Model mesh;
		Material material;

	public:
		MeshRenderer() = default;
		MeshRenderer(const MeshRenderer&) = default;
		MeshRenderer(const Model& mesh, const Material& material)
			: mesh(mesh), material(material) { }

		Model GetMesh() const { return mesh; }
		Material GetMaterial() const { return material; }

		void SetMesh(const Model& newMesh) { mesh = newMesh; }
		void SetMaterial(const Material& newMaterial) { material = newMaterial; }
	};
}