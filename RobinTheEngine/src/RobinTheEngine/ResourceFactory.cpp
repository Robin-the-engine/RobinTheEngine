#include "rtepch.h"
#include "ResourceFactory.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <functional>

int RTE::ResourceFactory::GetHashValue(std::string name)
{
	return 	std::hash<std::string> {}(name);

}

void RTE::ResourceFactory::ReadYamlKeys()
{

	YAML::Node node = YAML::LoadFile("utils\\Resources.yaml");

	for (auto i = node.begin(); i != node.end(); ++i) {

		yamlKeys[i->first.as<std::string>()] = i->second.as<std::string>();

	}

}
