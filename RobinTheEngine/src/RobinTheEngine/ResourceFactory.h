#pragma once
#include <string>
#include <unordered_map>
#include "Core.h"
#include "Platform/DirectX11/Material.h"
#include "Platform/DirectX11/Model.h"
#include "Platform/DirectX11/Shaders.h"
#include "Platform/DirectX11/Texture.h"
#include "MeshDescription.h"
#include "AI/BehaviourTree.h"

namespace RTE {

    class ResourceFactory
	{
	public:

		//Singleton
		inline static ResourceFactory& Get()
		{
			static ResourceFactory s_Instance;
			return s_Instance;
		}

		template<class T>
		T& GetResource(std::string key) {
			//We dont use that method. Its FALSE alwayse 	
			throw new std::exception("We dont use that method. Its FALSE always");
			//STATIC_ASSERT(false);
			//return T();
		}

		template<>
		Material& GetResource<Material>(std::string key) {

			RTE_CORE_ASSERT(materialDescriptors.find(key) != materialDescriptors.end(), "Dont have that key in material file!");
			auto desc = materialDescriptors[key];
			if (loadedResources.find(key) == loadedResources.end())
			{
				loadedResources[key] = new Material(desc);
			}
			return *((Material*)(loadedResources[key]));
		}


		template<>
		Texture& GetResource<Texture>(std::string key) {

			RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in material file!");
			auto path = yamlKeys[key];
			Texture* t = new Texture(path, aiTextureType::aiTextureType_DIFFUSE);
			return *t;
		}

		//template<>
		//pixelShader& GetResource<pixelShader>(std::string key) {

		//	RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in resource file!");
		//	auto path = yamlKeys[key];

		//	std::wstring convertedString(path.begin(), path.end());
		//	return pixelShader(convertedString);

		//	return (Texture&)loadedResources[key];
		//}

		//template<>
		//vertexShader& GetResource<vertexShader>(std::string key) {

		//	RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in resource file!");
		//	auto path = yamlKeys[key];


		//	const D3D11_INPUT_ELEMENT_DESC layout[] =
		//	{
		//		D3D11_INPUT_ELEMENT_DESC {"TEXCOORD",0,DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//		D3D11_INPUT_ELEMENT_DESC {"NORMAL", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		//		D3D11_INPUT_ELEMENT_DESC {"POSITION", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		//	};
		//	UINT numElements = ARRAYSIZE(layout);

		//	std::wstring convertedString(path.begin(), path.end());
		//	return vertexShader(convertedString, layout, numElements);
		//}

		/*
		template<>
		Model GetResource<Model>(std::string key) {

			Model model;
			//TODO: memory leak
			ConstantBuffer<CB_VS_MATRIX4x4>* cbuff = new ConstantBuffer<CB_VS_MATRIX4x4>(("MVPMatrix"));

			RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in resource file!");

			std::string path = yamlKeys[key];

			RTE_CORE_ASSERT(model.Initialize(path, *cbuff), "Cannot initialize model with that key");
			return model;
		}*/

		template<>
		Model& GetResource<Model>(std::string key) {

			//Model model;

			RTE_CORE_ASSERT(meshDescriptors.find(key) != meshDescriptors.end(), "Dont have that key in resource file!");
			auto desc = meshDescriptors[key];

			if (loadedResources.find(key) == loadedResources.end())
			{
				loadedResources[key] = Model::CreateModel(desc.key,desc.path, desc.layout);
			}
			return *((Model*)(loadedResources[key]));

			//RTE_CORE_ASSERT(model.Initialize(desc.path, desc.layout), "Cannot initialize model with that key");
			//return model;
		}

		int GetHashValue(std::string name);

		std::unordered_map<std::string, MeshDesc>& GetMeshDescriptorMap() { return meshDescriptors; }
		std::unordered_map<std::string, MaterialDescriptor>& GetMaterialDescriptorMap() { return materialDescriptors; }

	    template<>
		BehaviourTree& GetResource<BehaviourTree>(std::string key) {
			auto behav_tree_path = yamlKeys.find(key);
			RTE_CORE_ASSERT(behav_tree_path != yamlKeys.end(), "Dont have that key in resource file!");
			if (loadedResources.find(key) == loadedResources.end()) {
				loadedResources[key] = new BehaviourTree(behav_tree_path->second);
			}
			return dynamic_cast<BehaviourTree&>(*loadedResources[key]);
	    }

	private:
		void ReadYamlKeys();
		void ReadMaterialDescriptors();
		ResourceFactory() {
			ReadYamlKeys();
			ReadMaterialDescriptors();
		};


		// Its maps yaml keys to paths
		std::unordered_map<std::string, std::string> yamlKeys;
		std::unordered_map<std::string, MeshDesc> meshDescriptors;
		std::unordered_map<std::string, MaterialDescriptor> materialDescriptors;

		std::unordered_map<std::string, RTE::BaseResource*> loadedResources;
	};





}
