#pragma once
#include <unordered_map>
#include <string>
#include <d3d11.h>
#include "d3dUtils.h"

//#include "Platform/DirectX11/Shaders.h"
//#include "Platform/DirectX11/Texture.h"

namespace RTE
{

	class ComsManager
	{
	public:

		bool IsHaveComPtrResource(std::string name);

		/*template<class T>
		T GerResource(std::string yamlKey) {

			STATIC_ASSERT(false);//Dont Use that function!!
			return T();
		}

		template<>
		vertexShader GerResource<vertexShader>(std::string yamlKey) {

			std::wstring str(yamlKey.begin(), yamlKey.end());

			return vertexShader(str);
		}

		template<>
		pixelShader GerResource<pixelShader>(std::string yamlKey) {

			std::wstring str(yamlKey.begin(), yamlKey.end());

			return pixelShader(str);
		}

		template<>
		Texture GerResource<Texture>(std::string yamlKey) {

			return Texture(yamlKey, aiTextureType::aiTextureType_DIFFUSE);
		}*/

		/*template<>
		Model GerResource<Model>(std::string yamlKey) {

			Model model;
			ConstantBuffer<CB_VS_MATRIX4x4> cb("MVPMatrix");
			model.Initialize(yamlKey, cb);
			return  model;
		}*/


		template <class T>
		void RegisterComPtrResource(Microsoft::WRL::ComPtr<T> resPtr, std::string name)
		{
#ifdef _DEBUG
			RTE_ASSERT(!IsHaveComPtrResource(name), "Resource with that name already exist!")
#endif //debug
				resourceMap[name] = resPtr;

		}


		template <class T>
		Microsoft::WRL::ComPtr<T> GetComPtrResource(std::string name)
		{
			using namespace D3DUtils;
			auto resPtr = this->GetComPtrResource(name);
			Microsoft::WRL::ComPtr<T> resource;
			ThrowIfFailed(resPtr->QueryInterface(IID_PPV_ARGS(&resource)));
			return resource;
		}

		//Singleton pattern
		inline static ComsManager& Get()
		{
			static ComsManager s_rsInstance;
			return s_rsInstance;
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceChild> GetComPtrResource(std::string& name);
		ComsManager() {};
		ComsManager(const ComsManager&);
		ComsManager& operator=(ComsManager&);

		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11DeviceChild>> resourceMap;
	};


	/*
		template<>
		vertexShader ResourceManager::GerResource<vertexShader>(std::string yamlKey) {

			std::wstring str(yamlKey.begin(), yamlKey.end());

			return vertexShader(str);
		}

		template<>
		pixelShader ResourceManager::GerResource<pixelShader>(std::string yamlKey) {

			std::wstring str(yamlKey.begin(), yamlKey.end());

			return pixelShader(str);
		}
	*/

}
