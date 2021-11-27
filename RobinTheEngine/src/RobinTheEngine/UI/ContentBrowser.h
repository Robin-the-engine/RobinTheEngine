#pragma once
#include <filesystem>


namespace RTE {

    class ContentBrowser final {

    public:
        using content_iterator = std::filesystem::directory_iterator;

        enum ContentType {
            FILE,
            DIRECTORY,
        };

        ContentBrowser(const std::string& contentDir = DEFAULT_CONTENT_DIR);
        std::vector<std::pair<ContentType, std::string>> ListWorkingDirectory() const;


    private:
        std::filesystem::directory_entry currentWorkingDir;
        std::string contentDir;
        static const std::string DEFAULT_CONTENT_DIR;
    };
}
