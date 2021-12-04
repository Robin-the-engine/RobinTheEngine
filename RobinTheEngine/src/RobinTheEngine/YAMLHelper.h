#pragma once
#include <yaml-cpp/yaml.h>
#include "MeshDescription.h"

namespace RTE {
	class YamlHelper {
	public:
		static void ReadMeshes(YAML::Node& node, std::unordered_map<std::string, RTE::MeshDesc>& meshes);
		static void ReadGroup(YAML::Node& node, std::unordered_map<std::string, std::string>& group);
		static void ReadGroup(const std::string& fileName, const std::string& groupName, std::unordered_map<std::string, std::string>& group);
	};
}
