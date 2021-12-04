#include "rtepch.h"
#include "YAMLHelper.h"

#include "Core/RTECoreAsserts.h"

using namespace RTE;

void YamlHelper::ReadMeshes(YAML::Node& node, std::unordered_map<std::string, RTE::MeshDesc>& meshes) {
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

void YamlHelper::ReadGroup(YAML::Node& groupMap, std::unordered_map<std::string, std::string>& group) {
	RTE_CORE_ASSERT(groupMap.Type() == YAML::NodeType::Map, "Bad node");
	for (auto i = groupMap.begin(); i != groupMap.end(); i++) {
		std::string key = i->first.as<std::string>();
		std::string path = i->second.as<std::string>();
		if (group.find(key) != group.end()) { RTE_CORE_WARN("Key:{0} already exist in table!", key); }
		group[key] = path;
	}
}

void YamlHelper::ReadGroup(
	const std::string& fileName,
	const std::string& groupName,
	std::unordered_map<std::string, std::string>& group
) {
	YAML::Node node = YAML::LoadFile(fileName);
	auto a = node.Type();
	for (auto i = node.begin(); i != node.end(); ++i) {
		auto nodeName = i->first.as<std::string>();
		if (nodeName == groupName) {
			auto textureMap = i->second;
			ReadGroup(textureMap, group);
		}
	}
}

void YamlHelper::ReadResourceFile(
	const std::string& fileName,
    std::unordered_map<std::string, std::string>& fileContent,
    std::unordered_map<std::string, MeshDesc>& meshContent
) {
    YAML::Node node = YAML::LoadFile(fileName);
	for (auto i = node.begin(); i != node.end(); ++i) {

		auto group = i->first.as<std::string>();
		if (group == "meshes") {
			YamlHelper::ReadMeshes(i->second, meshContent);
		}
		else if (group == "textures") {
			auto textureMap = i->second;
			YamlHelper::ReadGroup(textureMap, fileContent);
		}
		else if (group == "shaders") {
			auto textureMap = i->second;
			YamlHelper::ReadGroup(textureMap, fileContent);
		}
		else {
			RTE_ERROR("That group dont exist in engine:" + group);
			RTE_CORE_ASSERT(false, "Bad resource group.");
		}
	}

}

