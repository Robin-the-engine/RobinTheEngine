#pragma once
#include "Platform/DirectX11/Mesh.h"
#include <string>
#include <memory>
#include "RobinTheEngine/Scene/BaseResource.h"


#include "assimp/scene.h"

using namespace DirectX;

namespace RTE {

	class Model: public BaseResource
	{
	public:
		Model() {}
		std::vector<std::shared_ptr<IMesh>> meshes;

		static Model* CreateModel(const std::string& path, int layout);


	private:
		bool LoadModel(const std::string& filePath, int lauout);
		void ProcessNode(aiNode* node, const aiScene* scene, int layout);
		std::shared_ptr<IMesh> ProcessMesh(aiMesh* mesh, int layout);

	};
}




