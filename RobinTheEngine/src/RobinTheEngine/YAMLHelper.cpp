#include "rtepch.h"
#include "YAMLHelper.h"
#include "Core/RTECoreAsserts.h"
#include <fstream>

using namespace RTE;

const std::string YamlHelper::MESH_GROUP = "meshes";
const std::string YamlHelper::SHADER_GROUP = "shaders";
const std::string YamlHelper::TEXTURE_GROUP = "textures";
const std::string YamlHelper::MESH_GROUP_KEY_LAYOUT = "layout";
const std::string YamlHelper::GROUP_KEY_PATH = "path";

const std::string YamlHelper::loggerName = "YamlHelper";


void YamlHelper::ReadMeshes(YAML::Node& node, std::unordered_map<std::string, RTE::MeshDesc>& meshes) {
    RTE_CORE_ASSERT(node.Type() == YAML::NodeType::Map, "Bad node");

    for (auto i = node.begin(); i != node.end(); i++) {

        std::string key = i->first.as<std::string>();

        auto body = i->second;
        RTE::MeshDesc descriptor;
        descriptor.key = key;

        for (auto j = body.begin(); j != body.end(); j++) {

            std::string param = j->first.as<std::string>();

            if (param == MESH_GROUP_KEY_LAYOUT) {
                descriptor.layout = j->second.as<int>();
            }
            else if (param == GROUP_KEY_PATH) {
                descriptor.path = j->second.as<std::string>();
            }
            else {
                Log::GetLogger(loggerName)->warn(std::format("Bad param in mesh:{}, with name {}", key, param));
            }
        }

        if (meshes.find(key) != meshes.end()) {
            Log::GetLogger(loggerName)->warn(std::format("Mesh with key:{} already exist in table!", key));
        }

        meshes[key] = descriptor;

    }
}

void YamlHelper::ReadMeshes(
    const std::string& fileName,
    const std::string& groupName,
    std::unordered_map<std::string, RTE::MeshDesc>& meshes
) {
    YAML::Node node = YAML::LoadFile(fileName);
    for (auto i = node.begin(); i != node.end(); ++i) {
        auto nodeName = i->first.as<std::string>();
        if (nodeName == groupName) {
            auto groupNode = i->second;
            ReadMeshes(groupNode, meshes);
        }
    }
}

void YamlHelper::ReadGroup(YAML::Node& groupMap, std::unordered_map<std::string, std::string>& group) {
    RTE_CORE_ASSERT(groupMap.Type() == YAML::NodeType::Map, "Bad node");
    for (auto i = groupMap.begin(); i != groupMap.end(); i++) {
        std::string key = i->first.as<std::string>();
        std::string path = i->second.as<std::string>();
        if (group.find(key) != group.end()) {
            Log::GetLogger(loggerName)->warn(std::format("Key:{} already exist in table!!", key));
        }
        group[key] = path;
    }
}

void YamlHelper::ReadGroup(
    const std::string& fileName,
    const std::string& groupName,
    std::unordered_map<std::string, std::string>& group
) {
    YAML::Node node = YAML::LoadFile(fileName);
    for (auto i = node.begin(); i != node.end(); ++i) {
        auto nodeName = i->first.as<std::string>();
        if (nodeName == groupName) {
            auto groupNode = i->second;
            ReadGroup(groupNode, group);
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
        if (group == MESH_GROUP) {
            YamlHelper::ReadMeshes(i->second, meshContent);
        }
        else if (group == TEXTURE_GROUP) {
            auto textureMap = i->second;
            YamlHelper::ReadGroup(textureMap, fileContent);
        }
        else if (group == SHADER_GROUP) {
            auto textureMap = i->second;
            YamlHelper::ReadGroup(textureMap, fileContent);
        }
        else {
            Log::GetLogger(loggerName)->warn(std::format("That group don't exist in engine: {}", group));
            RTE_CORE_ASSERT(false, "Bad resource group.");
        }
    }

}

void YamlHelper::UpdateResourceFile(
    const std::string& fileName,
    const std::unordered_map<std::string, std::string>& shaders,
    const std::unordered_map<std::string, std::string>& textures,
    const std::unordered_map<std::string, RTE::MeshDesc>& meshes
) {
    std::ofstream of(fileName);
    YAML::Emitter out(of);
    UpdateResourceGroup(out, YamlHelper::TEXTURE_GROUP, textures);
    UpdateResourceGroup(out, YamlHelper::SHADER_GROUP, shaders);
    UpdateMeshGroup(out, YamlHelper::MESH_GROUP, meshes);
}

void YamlHelper::UpdateResourceGroup(
    YAML::Emitter& out,
    const std::string groupName,
    const std::unordered_map<std::string, std::string>& group
) {
    YAML::Node yamlGroup;
    for (const auto& e : group) {
        yamlGroup[e.first] = e.second;
    }
    YAML::Node mainNode;
    mainNode[groupName] = yamlGroup;
    out << mainNode << YAML::Newline;
}

void YamlHelper::UpdateMeshGroup(
    YAML::Emitter& out,
    const std::string groupName,
    const std::unordered_map<std::string, RTE::MeshDesc>& group
) {
    YAML::Node meshTag;
    YAML::Node mainNode;

    for (const auto& e : group) {
        YAML::Node descNode;
        descNode[MESH_GROUP_KEY_LAYOUT] = e.second.layout;
        descNode[GROUP_KEY_PATH] = e.second.path;
        meshTag[e.first] = descNode;
    }
    mainNode[groupName] = meshTag;
    out << mainNode << YAML::Newline;

}
