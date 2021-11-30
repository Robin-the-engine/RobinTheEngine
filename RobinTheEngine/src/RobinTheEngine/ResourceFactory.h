#pragma once
#include <string>
#include <unordered_map>
#include "Core.h"
#include "Platform/DirectX11/Material.h"
#include "Platform/DirectX11/Model.h"
#include "Platform/DirectX11/Shaders.h"
#include "Platform/DirectX11/Texture.h"



namespace RTE {

	struct MeshDesc
	{
		std::string key;
		int layout;
		std::string path;
		MeshDesc() :layout(-1), path(""), key("") {}

	};

	class ResourceFactory
	{
	public:

		template<class T>
		static T& GetResource(std::string key) {
			//We dont use that method. Its FALSE alwayse 	
			STATIC_ASSERT(false);
			throw new std::exception("We dont use that method. Its FALSE alwayse");
		}

		template<>
		static Material& GetResource<Material>(std::string key) {

			RTE_CORE_ASSERT(materialDescriptors.find(key) != materialDescriptors.end(), "Dont have that key in material file!");
			auto desc = materialDescriptors[key];
			if (loadedResources.find(key) == loadedResources.end())
			{
				loadedResources[key] = new Material(desc);
			}
			return *((Material*)(loadedResources[key]));
		}


		template<>
		static Texture& GetResource<Texture>(std::string key) {

			RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in material file!");
			auto path = yamlKeys[key];
			Texture* t = new Texture(path, aiTextureType::aiTextureType_DIFFUSE);
			return *t;
		}

		template<>
		static Model& GetResource<Model>(std::string key) {

			//Model model;

			RTE_CORE_ASSERT(meshDescriptors.find(key) != meshDescriptors.end(), "Dont have that key in resource file!");
			auto desc = meshDescriptors[key];

			if (loadedResources.find(key) == loadedResources.end())
			{
				loadedResources[key] = Model::CreateModel(desc.path, desc.layout);
			}
			return *((Model*)(loadedResources[key]));

			//RTE_CORE_ASSERT(model.Initialize(desc.path, desc.layout), "Cannot initialize model with that key");
			//return model;
		}

		static int GetHashValue(std::string name);

		static void Init();

	private:

		static void LoadResourcesList();
		static void ReadMaterialDescriptors();

		static std::unordered_map<std::string, std::string> yamlKeys; // Map of all resources [resource id, path to resource]
		static std::unordered_map<std::string, RTE::BaseResource*> loadedResources; // Map of loaded resources [resource id, pointer to resource]

		static std::unordered_map<std::string, MeshDesc> meshDescriptors;
		static std::unordered_map<std::string, MaterialDescriptor> materialDescriptors;
	};





}
