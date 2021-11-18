#pragma once
#include "RobinTheEngine/Scene/Component.h"
#include <DirectXMath.h>
#include "RobinTheEngine/Scene/BaseResource.h"

#include "Platform/DirectX11/Model.h"
#include "Platform/DirectX11/Material.h"


namespace RTE
{
#pragma region Dymmy

	/*
	//  TEMP: Resource manager interface
	class ResourceManager
	{
	public:
		template<typename T>
		static T Get(std::string id)
		{
			T result(id);
			return result;
		}
	};*/

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
		virtual void Draw(const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewProjectionMatrix) {};
	};



	//  TEMP: Abstract Material class
	class IMaterial : public BaseResource { public: IMaterial(ResourceID id) : BaseResource(id) { } };
*/
#pragma endregion

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