#pragma once
#include <filesystem>
#include <utility>
#include "../Log.h"

namespace RTE {
    struct MeshDesc;

    class ContentBrowser final {

    public:
        using content_iterator = std::filesystem::directory_iterator;

        enum ContentType {
            UNKNOWNN,
            FILE,
            DIRECTORY,
            MESH,
            TEXTURE,
            SHADER,
        };

        ContentBrowser();
        ContentBrowser(const std::string& contentFile);

        void setContentFile(const std::string& contentFile);
        std::vector<std::pair<ContentType, std::string>>
        listDirectory(const std::string& directory = "") const;
        void addFile(const std::string& filepath, const std::string directory, ContentType type);
        std::pair<bool, std::error_code> removeFile(const std::string directory, const std::string& fileName);

        std::string getNextDir(const std::string& current, const std::string& next);

    private:
        void init();
        void scanContentFile();
        void scanContentGroup(const std::string &groupName, std::unordered_map<std::string, std::string>& group);
        void scanMeshGroup(const std::string &groupName, std::unordered_map<std::string, MeshDesc>& group);
        void updateContentFile(ContentType type) const;
        ContentType removeAndGetType(const std::string filepath);
        std::string contentFile;
        Log::SPloggerT logger;
        std::unordered_map<std::string, std::string> textures;
        std::unordered_map<std::string, std::string> shaders;
        std::unordered_map<std::string, MeshDesc> meshes;
    };

    class ContentBrowserGUI final {
    public:
        static const std::string openFileDialog();
    };

}
