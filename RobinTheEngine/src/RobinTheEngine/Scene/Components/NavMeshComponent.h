#pragma once
#include "RobinTheEngine/Scene/BaseResource.h"
#include "RobinTheEngine/Scene/Component.h"
#include "DetourNavMesh.h"

namespace RTE
{

	struct NavMeshImpl final: public BaseResource {
		NavMeshImpl(const std::string& path, int flags = 0);
		//std::vector<unsigned char> data;
		//int flags;
		std::string path;
	};

    class NavMeshComponent final: public Component {
	public:
		NavMeshComponent() = default;
		NavMeshComponent(const std::string& navMeshKey);
		void init(const std::string& navMeshKey);
		dtNavMesh* get();
	private:
		dtNavMesh* navmesh;
	};
}
