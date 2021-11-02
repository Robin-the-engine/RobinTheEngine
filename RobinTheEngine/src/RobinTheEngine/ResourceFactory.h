#pragma once
#include <string>
#include <unordered_map>
#include "Core.h"
#include "Platform/DirectX11/Material.h"
#include "Platform/DirectX11/Model.h"
#include "Platform/DirectX11/Shaders.h"




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
		T GetResource(std::string key) {
			//We dont use that method. Its FALSE alwayse 	
			STATIC_ASSERT(false);
			return T();
		}

		template<>
		Material GetResource<Material>(std::string key) {

			RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in resource file!");
			auto stringKey = yamlKeys[key];
			int intKey = std::atoi(stringKey.c_str());
			RTE_CORE_ASSERT(intKey, "Cannot convert yaml key to int or its equal to zero.");

			switch (intKey)
			{
			default:
			case 1:
				return DefaultMaterial();
			}

		}

		template<>
		pixelShader GetResource<pixelShader>(std::string key) {

			RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in resource file!");
			auto path = yamlKeys[key];

			std::wstring convertedString(path.begin(), path.end());
			return pixelShader(convertedString);

		}

		template<>
		vertexShader GetResource<vertexShader>(std::string key) {

			RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in resource file!");
			auto path = yamlKeys[key];


			const D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				D3D11_INPUT_ELEMENT_DESC {"TEXCOORD",0,DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
				D3D11_INPUT_ELEMENT_DESC {"NORMAL", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
				D3D11_INPUT_ELEMENT_DESC {"POSITION", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			UINT numElements = ARRAYSIZE(layout);

			std::wstring convertedString(path.begin(), path.end());
			return vertexShader(convertedString, layout, numElements);

		}

		template<>
		Model GetResource<Model>(std::string key) {

			Model model;
			ConstantBuffer<CB_VS_MATRIX4x4> cbuff("LightProps");

			RTE_CORE_ASSERT(yamlKeys.find(key) != yamlKeys.end(), "Dont have that key in resource file!");

			std::string path = yamlKeys[key];

			RTE_CORE_ASSERT(model.Initialize(path, cbuff), "Cannot initialize model with that key");
			return model;
		}

		int GetHashValue(std::string name);

	private:
		void ReadYamlKeys();
		ResourceFactory() {
			ReadYamlKeys();
		};

		// Its maps yaml keys to paths
		std::unordered_map<std::string, std::string> yamlKeys;

	};





}
