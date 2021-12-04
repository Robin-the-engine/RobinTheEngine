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

			else {
				RTE_CORE_WARN("Bad param in material:{0}, with name:{1}", key, param);
			}
		}

		if (materialDescriptors.find(key) != materialDescriptors.end()) { RTE_CORE_WARN("Material with key:{0} already exist in table!", key); }

		materialDescriptors[key] = descriptor;

	}
}
