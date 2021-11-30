#include "rtepch.h"
#include "ResourceFactory.h"
#include <fstream>
#include <functional>
#include "yaml-cpp/yaml.h"
#include <cassert>

// Init unordered map's
using PathMap = std::unordered_map<std::string, std::string>;
using LoadedMap = std::unordered_map<std::string, RTE::BaseResource*>;
using MeshMap = std::unordered_map<std::string, RTE::MeshDesc>;
using MatMap = std::unordered_map<std::string, RTE::MaterialDescriptor>;

PathMap RTE::ResourceFactory::yamlKeys = PathMap();
LoadedMap RTE::ResourceFactory::loadedResources = LoadedMap();
MeshMap RTE::ResourceFactory::meshDescriptors = MeshMap();
MatMap RTE::ResourceFactory::materialDescriptors = MatMap();

void ReadMeshes(YAML::Node& node, std::unordered_map<std::string, RTE::MeshDesc>& meshes) {

	RTE_CORE_ASSERT(node.Type() == YAML::NodeType::Map, "Bad node");

	for (auto i = node.begin(); i != node.end(); i++) {

		std::string key = i->first.as<std::string>();

		auto body = i->second;
		RTE::MeshDesc descriptor;
		descriptor.key = key;

		for (auto j = body.begin(); j != body.end(); j++) {

			std::string param = j->first.as<std::string>();

			if (param == "layout") {
				descriptor.layout = j->second.as<int>();
			}
			else if (param == "path") {
				descriptor.path = j->second.as<std::string>();
			}
			else {
				RTE_CORE_WARN("Bad param in mesh:{0}, with name {1}", key, param);
			}
		}

		if (meshes.find(key) != meshes.end()) { RTE_CORE_WARN("Mesh with key:{0} already exist in table!", key); }

		meshes[key] = descriptor;

	}

}



int RTE::ResourceFactory::GetHashValue(std::string name)
{
	return std::hash<std::string> {}(name);
}

void RTE::ResourceFactory::Init()
{
	LoadResourcesList();
	ReadMaterialDescriptors();
};

void RTE::ResourceFactory::LoadResourcesList()
{

	YAML::Node node = YAML::LoadFile("utils\\Resources.yaml");
	auto a = node.Type();
	for (auto i = node.begin(); i != node.end(); ++i) {

		auto group = i->first.as<std::string>();
		if (group == "meshes") {
			ReadMeshes(i->second, meshDescriptors);
		}
		else if (group == "textures") {
			auto textureMap = i->second;
			RTE_CORE_ASSERT(textureMap.Type() == YAML::NodeType::Map, "Bad node");
			for (auto i = textureMap.begin(); i != textureMap.end(); i++) {
				std::string key = i->first.as<std::string>();
				std::string path = i->second.as<std::string>();

				if (yamlKeys.find(key) != yamlKeys.end()) { RTE_CORE_WARN("Texture with key:{0} already exist in table!", key); }
				yamlKeys[key] = path;
			}
		}
		else if (group == "shaders") {
			auto textureMap = i->second;
			RTE_CORE_ASSERT(textureMap.Type() == YAML::NodeType::Map, "Bad node");
			for (auto i = textureMap.begin(); i != textureMap.end(); i++) {
				std::string key = i->first.as<std::string>();
				std::string path = i->second.as<std::string>();

				if (yamlKeys.find(key) != yamlKeys.end()) { RTE_CORE_WARN("Shader with key:{0} already exist in table!", key); }
				yamlKeys[key] = path;
			}
		}

		else {
			RTE_ERROR("That group dont exist in engine:" + group);
			RTE_CORE_ASSERT(false, "Bad resource group.");
		}

		//yamlKeys[i->first.as<std::string>()] = i->second.as<std::string>();

	}

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
				descriptor.vsPath = yamlKeys[descriptor.vsKey];

			}
			else if (param == "psshader") {
				descriptor.psKey = j->second.as<std::string>();
				RTE_CORE_ASSERT(yamlKeys.find(descriptor.psKey) != yamlKeys.end(), "Cannot find {0} with name {1}", param, descriptor.key);
				descriptor.psPath = yamlKeys[descriptor.psKey];
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

