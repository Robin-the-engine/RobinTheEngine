#pragma once
#include "RobinTheEngine/Scene/Component.h"

namespace RTE
{
	//  TEMP: Resource manager interface
	class ResourceManager
	{
	public:
		template<typename T>
		static T Get(std::string);
	};
	//  TEMP: Base resource class
	class BaseResource
	{
		using ResourceID = std::string;
	private:
		const ResourceID id;
	public:
		BaseResource() : id("TODO") {}
		BaseResource(ResourceID id) : id(id) {}

		const ResourceID GetResourceID() const;
	};

	// TEMP: Mesh interface
	class IMesh : public BaseResource
	{
	public:
		void operator=(const IMesh&);
	};
	// TEMP: Material interface
	class IMaterial : public BaseResource
	{
	public:
		void operator=(const IMaterial&);
	};


	struct MeshRenderer: public Component
	{
	private:
		IMesh mesh;
		IMaterial material;

	public:
		MeshRenderer(const MeshRenderer&) = default;
		MeshRenderer(const IMesh& mesh, const IMaterial& material)
			: mesh(mesh), material(material) { }

		IMesh GetMesh() const { return mesh; }
		IMaterial GetMaterial() const { return material; }

		void SetMesh(const IMesh& newMesh) { mesh = newMesh; }
		IMaterial SetMaterial(const IMaterial& newMaterial) { material = newMaterial; }
	};
}