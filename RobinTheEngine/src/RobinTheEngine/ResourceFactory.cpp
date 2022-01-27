#include "rtepch.h"
#include "ResourceFactory.h"
#include <fstream>
#include <functional>
#include <cassert>
#include "YAMLHelper.h"

int RTE::ResourceFactory::GetHashValue(std::string name)
{
	return 	std::hash<std::string> {}(name);

}

void RTE::ResourceFactory::ReadYamlKeys()
{
	YamlHelper::ReadResourceFile("utils\\Resources.yaml", yamlKeys, meshDescriptors);
}

void RTE::ResourceFactory::ReadMaterialDescriptors()
{
	YAML::Node node = YAML::LoadFile("utils\\Materials.yaml");

	for (auto i = node.begin(); i != node.end(); ++i) {

		std::string key = i->first.as<std::string>();

		auto body = i->second;
		RTE::MaterialDescriptor descriptor;
		descriptor.key = key;

		for (auto j = body.begin(); j != body.end(); j++) {

			std::string param = j->first.as<std::string>();

			if (param == "materialType") {
				descriptor.MaterialType = j->second.as<int>();
			}
			else if (param == "vsshader") {
				descriptor.vsKey = j->second.as<std::string>();
				RTE_CORE_ASSERT(yamlKeys.find(descriptor.vsKey) != yamlKeys.end(), "Cannot find {0} with name {1}", param, descriptor.key);
				descriptor.vsPath = this->yamlKeys[descriptor.vsKey];

			}
			else if (param == "psshader") {
				descriptor.psKey = j->second.as<std::string>();
				RTE_CORE_ASSERT(yamlKeys.find(descriptor.psKey) != yamlKeys.end(), "Cannot find {0} with name {1}", param, descriptor.key);
				descriptor.psPath = this->yamlKeys[descriptor.psKey];
			}
			else if (param == "textureKey") {
				descriptor.textureKey = j->second.as<std::string>();
			}
			else if (param == "color") {
				auto colorBody = j->second;

				for (auto colorComponent = colorBody.begin(); colorComponent != colorBody.end(); ++colorComponent) {
					auto component = colorComponent->first.as<std::string>();
					auto value = colorComponent->second.as<float>();
					if (component == "r") descriptor.color[0] = value;
					else if (component == "g") descriptor.color[1] = value;
					else if (component == "b") descriptor.color[2] = value;
					else if (component == "a") descriptor.color[3] = value;
					else RTE_CORE_WARN("Unknown color component in material:{0} with key:{1}", key, component);


				}

			}
			else if (param == "ambientColor") {
				auto colorBody = j->second;

				for (auto colorComponent = colorBody.begin(); colorComponent != colorBody.end(); ++colorComponent) {
					auto component = colorComponent->first.as<std::string>();
					auto value = colorComponent->second.as<float>();
					if (component == "r") descriptor.AmbientColor.x = value;
					else if (component == "g") descriptor.AmbientColor.y = value;
					else if (component == "b") descriptor.AmbientColor.z = value;
					else if (component == "a") descriptor.AmbientColor.w = value;
					else RTE_CORE_WARN("Unknown color component in material:{0} with key:{1}", key, component);


				}
			}
			else if (param == "emissiveColor") {
				auto colorBody = j->second;

				for (auto colorComponent = colorBody.begin(); colorComponent != colorBody.end(); ++colorComponent) {
					auto component = colorComponent->first.as<std::string>();
					auto value = colorComponent->second.as<float>();
					if (component == "r") descriptor.EmissiveColor.x = value;
					else if (component == "g") descriptor.EmissiveColor.y = value;
					else if (component == "b") descriptor.EmissiveColor.z = value;
					else if (component == "a") descriptor.EmissiveColor.w = value;
					else RTE_CORE_WARN("Unknown color component in material:{0} with key:{1}", key, component);


				}
			}
			else if (param == "emissiveColor") {
				auto colorBody = j->second;

				for (auto colorComponent = colorBody.begin(); colorComponent != colorBody.end(); ++colorComponent) {
					auto component = colorComponent->first.as<std::string>();
					auto value = colorComponent->second.as<float>();
					if (component == "r") descriptor.EmissiveColor.x = value;
					else if (component == "g") descriptor.EmissiveColor.y = value;
					else if (component == "b") descriptor.EmissiveColor.z = value;
					else if (component == "a") descriptor.EmissiveColor.w = value;
					else RTE_CORE_WARN("Unknown color component in material:{0} with key:{1}", key, component);
				}
			}
			else if (param == "diffuseColor") {
				auto colorBody = j->second;
				for (auto colorComponent = colorBody.begin(); colorComponent != colorBody.end(); ++colorComponent) {
					auto component = colorComponent->first.as<std::string>();
					auto value = colorComponent->second.as<float>();
					if (component == "r") descriptor.DiffuseColor.x = value;
					else if (component == "g") descriptor.DiffuseColor.y = value;
					else if (component == "b") descriptor.DiffuseColor.z = value;
					else if (component == "a") descriptor.DiffuseColor.w = value;
					else RTE_CORE_WARN("Unknown color component in material:{0} with key:{1}", key, component);
				}
			}
			else if (param == "specularColor") {
				auto colorBody = j->second;
				for (auto colorComponent = colorBody.begin(); colorComponent != colorBody.end(); ++colorComponent) {
					auto component = colorComponent->first.as<std::string>();
					auto value = colorComponent->second.as<float>();
					if (component == "r") descriptor.SpecularColor.x = value;
					else if (component == "g") descriptor.SpecularColor.y = value;
					else if (component == "b") descriptor.SpecularColor.z = value;
					else if (component == "a") descriptor.SpecularColor.w = value;
					else RTE_CORE_WARN("Unknown color component in material:{0} with key:{1}", key, component);
				}
			}
			else if (param == "reflectance") {
				auto colorBody = j->second;
				for (auto colorComponent = colorBody.begin(); colorComponent != colorBody.end(); ++colorComponent) {
					auto component = colorComponent->first.as<std::string>();
					auto value = colorComponent->second.as<float>();
					if (component == "r") descriptor.Reflectance.x = value;
					else if (component == "g") descriptor.Reflectance.y = value;
					else if (component == "b") descriptor.Reflectance.z = value;
					else if (component == "a") descriptor.Reflectance.w = value;
					else RTE_CORE_WARN("Unknown color component in material:{0} with key:{1}", key, component);
				}
			}
			else if (param == "opacity") {
				descriptor.Opacity = j->second.as<float>();
			}
			else if (param == "specularPower") {
			descriptor.SpecularPower = j->second.as<float>();
			}
			else if (param == "indexOfRefraction") {
			descriptor.IndexOfRefraction = j->second.as<float>();
			}
			else if (param == "bumpIntensity") {
			descriptor.BumpIntensity = j->second.as<float>();
			}
			else if (param == "specularScale") {
			descriptor.SpecularScale = j->second.as<float>();
			}
			else if (param == "alphaThreshold") {
			descriptor.AlphaThreshold = j->second.as<float>();
			}
			else if (param == "ambientTexture") {
			descriptor.AmbientTextureKey = j->second.as<std::string>();
			}
			else if (param == "emissiveTexture") {
			descriptor.EmissiveTextureKey = j->second.as<std::string>();
			}
			else if (param == "diffuseTexture") {
			descriptor.DiffuseTextureKey = j->second.as<std::string>();
			}
			else if (param == "specularTexture") {
			descriptor.SpecularTextureKey = j->second.as<std::string>();
			}
			else if (param == "specularPowerTexture") {
			descriptor.SpecularPowerTextureKey = j->second.as<std::string>();
			}
			else if (param == "normalTexture") {
			descriptor.NormalTextureKey = j->second.as<std::string>();
			}
			else if (param == "bumpTexture") {
			descriptor.BumpTextureKey = j->second.as<std::string>();
			}
			else if (param == "opacityTexture") {
			descriptor.OpacityTextureKey = j->second.as<std::string>();
			}


			else {
				RTE_CORE_WARN("Bad param in material:{0}, with name:{1}", key, param);
			}
		}

		if (materialDescriptors.find(key) != materialDescriptors.end()) { RTE_CORE_WARN("Material with key:{0} already exist in table!", key); }

		materialDescriptors[key] = descriptor;

	}
}



/*
ambientColor
emissiveColor
diffuseColor
specularColor
opacity
specularPower
indexOfRefraction
bumpIntensity
specularScale
alphaThreshold
ambientTexture
emissiveTexture
diffuseTexture
specularTexture
specularPowerTexture
normalTexture
bumpTexture
opacityTexture



*/
