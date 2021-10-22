#pragma once
#include <string>
#include "Core.h"
#include "Platform/DirectX11/Material.h"
#include "Platform/DirectX11/Model.h"



namespace RTE {


	/*
	Usage:
		#include "RobinTheEngine/ReosurceFactory.h"
	
		//Right now its work only with file path
		Model model = ResourceFactory::Get().GetResource<Model>("objects\\ogre\\bs_rest.obj");
	
	*/


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
		T GetResource(std::string key){ 
			//We dont use that method. Its FALSE alwayse 	
			STATIC_ASSERT(false); 
			return T();
		}

		template<>
		Material GetResource<Material>(std::string key) {
			//TODO: its temp method. need real emplementation! 
			return Material();
		}

		//пока что передаю модель. хз стоит ли отдавать отдельные куски меша т.к. 
		//у них может не быть названия и получается брать их можно будет только по индексу
		template<>
		Model GetResource<Model>(std::string key) {

			Model model;
			ConstantBuffer<CB_VS_MATRIX4x4> cbuff("LightProps");

			RTE_CORE_ASSERT( model.Initialize(key, cbuff), "Cannot initialize model with that key");
			return model;
		}
		

	private:
		void ReadYamlKeys() {};
		ResourceFactory() {};

	};





}
