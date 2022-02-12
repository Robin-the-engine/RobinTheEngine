#include "rtepch.h"
#include "NavMeshComponent.h"
#include "../../ResourceFactory.h"
#include <fstream>
#include <vector>
#include <filesystem>

using namespace RTE;

static constexpr  int NAVMESHSET_MAGIC = 'M'<<24 | 'S'<<16 | 'E'<<8 | 'T'; //'MSET';
static constexpr int NAVMESHSET_VERSION = 1;

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

static dtNavMesh* loadAll(const char* path)
{
	FILE* fp = fopen(path, "rb");
	if (!fp) return 0;

	// Read header.
	NavMeshSetHeader header;
	size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (readLen != 1)
	{
		fclose(fp);
		return 0;
	}
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return 0;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return 0;
	}

	dtNavMesh* mesh = dtAllocNavMesh();
	if (!mesh)
	{
		fclose(fp);
		return 0;
	}
	dtStatus status = mesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return 0;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
			return 0;
		}

		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		readLen = fread(data, tileHeader.dataSize, 1, fp);
		if (readLen != 1)
		{
			dtFree(data);
			fclose(fp);
			return 0;
		}

		mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	fclose(fp);

	return mesh;
}

//NavMeshImpl::NavMeshImpl(const std::string& path, int flags) : flags(flags) {
//	auto sz = std::filesystem::file_size(path);
//	std::ifstream input(path, std::ios::binary);
//	const std::vector<unsigned char> tmpdata{ std::istreambuf_iterator(input), {} };
//	data = tmpdata;
//}

NavMeshImpl::NavMeshImpl(const std::string& path, int flags) : path(path) {}

NavMeshComponent::NavMeshComponent(const std::string& navMeshKey) {
	init(navMeshKey);
}

void NavMeshComponent::init(const std::string& navMeshKey) {
    auto navMeshImpl = ResourceFactory::Get().GetResource<NavMeshImpl>(navMeshKey);
	navmesh = loadAll(navMeshImpl.path.c_str());
}

dtNavMesh* NavMeshComponent::get() {
    return navmesh;
}
