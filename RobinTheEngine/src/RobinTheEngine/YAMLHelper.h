#pragma once
#include <yaml-cpp/yaml.h>
#include "MeshDescription.h"

namespace RTE {
    class YamlHelper {
        static const std::string loggerName;
    public:

        static const std::string MESH_GROUP;
        static const std::string MESH_GROUP_KEY_LAYOUT;
        static const std::string TEXTURE_GROUP;
        static const std::string SHADER_GROUP;
        static const std::string GROUP_KEY_PATH;


        static void ReadMeshes(YAML::Node& node, std::unordered_map<std::string, RTE::MeshDesc>& meshes);
        static void ReadMeshes(
            const std::string& fileName,
            const std::string& groupName,
            std::unordered_map<std::string, RTE::MeshDesc>& meshes
        );
        static void ReadGroup(YAML::Node& node, std::unordered_map<std::string, std::string>& group);
        static void ReadGroup(
            const std::string& fileName,
            const std::string& groupName,
            std::unordered_map<std::string, std::string>& group
        );
        static void ReadResourceFile(
            const std::string& fileName,
            std::unordered_map<std::string, std::string>& fileContent,
            std::unordered_map<std::string, MeshDesc>& meshContent
        );
        static void UpdateResourceFile(
            const std::string& fileName,
            const std::unordered_map<std::string, std::string>& shaders,
            const std::unordered_map<std::string, std::string>& textures,
            const std::unordered_map<std::string, RTE::MeshDesc>& meshes
        );

        static void UpdateResourceGroup(
            YAML::Emitter& out,
            const std::string groupName,
            const std::unordered_map<std::string, std::string>& group
        );

        static void UpdateMeshGroup(
            YAML::Emitter& out,
            const std::string groupName,
            const std::unordered_map<std::string, RTE::MeshDesc>& group
        );
    };
}
