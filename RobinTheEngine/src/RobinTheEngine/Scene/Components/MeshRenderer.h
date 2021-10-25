#pragma once
#include "RobinTheEngine/Scene/Component.h"

namespace RTE
{
#pragma region Dymmy

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
	};
	//  TEMP: Base resource class
	class BaseResource
	{
	public:
		using ResourceID = std::string;
	private:
		ResourceID id = "invalid";
	public:
		BaseResource() = default;
		BaseResource(const BaseResource&) = default;

		BaseResource(ResourceID id) : id(id) { }


		const ResourceID GetResourceID() const { return id; }
	};
	//  TEMP: Abstract Mesh class
	class IMesh : public BaseResource { public: IMesh(ResourceID id) : BaseResource(id) { } };
	//  TEMP: Abstract Material class
	class IMaterial : public BaseResource { public: IMaterial(ResourceID id) : BaseResource(id) { } };

#pragma endregion

	struct MeshRenderer: public Component
	{
	private:
		IMesh mesh;
		IMaterial material;

	public:
		MeshRenderer() = default;
		MeshRenderer(const MeshRenderer&) = default;
		MeshRenderer(const IMesh& mesh, const IMaterial& material)
			: mesh(mesh), material(material) { }

		IMesh GetMesh() const { return mesh; }
		IMaterial GetMaterial() const { return material; }

		void SetMesh(const IMesh& newMesh) { mesh = newMesh; }
		void SetMaterial(const IMaterial& newMaterial) { material = newMaterial; }
	};
}