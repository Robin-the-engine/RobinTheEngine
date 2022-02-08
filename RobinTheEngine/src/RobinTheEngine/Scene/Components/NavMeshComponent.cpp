#include "rtepch.h"
#include "NavMeshComponent.h"
#include "../../ResourceFactory.h"
#include <fstream>
#include <vector>
#include <filesystem>

using namespace RTE;

NavMeshImpl::NavMeshImpl(const std::string& path, int flags): flags(flags) {
    auto sz = std::filesystem::file_size(path);
    std::ifstream input(path, std::ios::binary);
    const std::vector<unsigned char> tmpdata{ std::istreambuf_iterator(input), {} };
    data = tmpdata;
}

NavMeshComponent::NavMeshComponent(const std::string& navMeshKey) {
    init(navMeshKey);
}

void NavMeshComponent::init(const std::string& navMeshKey) {
    auto navMeshImpl = ResourceFactory::Get().GetResource<NavMeshImpl>(navMeshKey);
    navmesh.init(navMeshImpl.data.data(), navMeshImpl.data.size(), navMeshImpl.flags);
}

dtNavMesh* NavMeshComponent::get() {
    return &navmesh;
}
