#pragma once
#include "RobinTheEngine/Core.h"
#include "RobinTheEngine/Scene/BaseResource.h"
#include <d3d11.h>
#include "Platform/DirectX11/Mesh.h"

namespace RTE {

	// Interface class for meshes
	class IDrawable : public BaseResource {

	public:

		IDrawable(ResourceID id) : BaseResource(id) { }
		IDrawable() : BaseResource("invalid") {};
		std::vector<std::shared_ptr<IMesh>> GetMeshes() {};
	};

}
