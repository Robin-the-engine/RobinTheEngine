#include "rtepch.h"
#include "ContentBrowser.h"
#include "../Log.h"
#include <format>

using namespace RTE;
namespace fs = std::filesystem;

const std::string ContentBrowser::DEFAULT_CONTENT_DIR = "Sandbox/Content";

ContentBrowser::ContentBrowser(const std::string& contentDir): currentWorkingDir(contentDir), contentDir(contentDir) {
    assert(fs::is_directory(contentDir) && "Provided assets directory do not exists or not a directory!");
}

std::vector<std::pair<ContentBrowser::ContentType, std::string>> ContentBrowser::ListWorkingDirectory() const {
    std::vector<std::pair<ContentType, std::string>> dirContent;
    for(auto &e: fs::directory_iterator(currentWorkingDir)) {
        ContentType t;
        auto name = e.path().filename().generic_string();
        switch (e.status().type()) {
        case fs::file_type::regular:
            t = FILE; break;
        case fs::file_type::directory:
            t = DIRECTORY; break;
        default:
            Log::GetLogger("ContentBrowser")->error(std::format("Unsupported file type: {}", name));
            assert(false);
        }
        dirContent.emplace_back(t, name);
    }

    return dirContent;
}

