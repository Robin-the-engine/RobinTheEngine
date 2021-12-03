#pragma once
#include <filesystem>
#include <utility>
#include "../Log.h"


namespace RTE {

    class ContentBrowser final {

    public:
        using content_iterator = std::filesystem::directory_iterator;

        enum ContentType {
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
        void addFile(const std::string& filepath, const std::string directory, ContentType type) const;
        std::pair<bool, std::error_code> removeFile(const std::string& path) const;

        std::string getNextDir(const std::string& current, const std::string& next);

    private:
        void init();
        void checkContentFile();

        std::string contentFile;
        Log::SPloggerT logger;
    };

    class ContentBrowserGUI final {
    public:
        static const std::string openFileDialog();
    };

}
