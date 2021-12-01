#include "rtepch.h"
#include "ContentBrowser.h"
#include "yaml-cpp/yaml.h"
#include <format>

using namespace RTE;
namespace fs = std::filesystem;

ContentBrowser::ContentBrowser() {
    init();
}

ContentBrowser::ContentBrowser(const std::string& contentFile) {
    init();
    setContentFile(contentFile);
}

void ContentBrowser::setContentFile(const std::string& contentFile) {
    assert(fs::is_regular_file(contentFile) && "Provided path do not exists or not a file!");
    this->contentFile = contentFile;
    logger->trace(
        std::format("Set content file to: {}"), fs::absolute(contentFile).generic_string()
    );
    checkContentFile();
}

std::vector<std::pair<ContentBrowser::ContentType, std::string>>
ContentBrowser::listDirectory(const std::string& directory) const {
    fs::path workingDir;
    if(directory.empty()) {
        workingDir = fs::current_path();
    }
    else {
        workingDir = fs::absolute(directory);
    }
    std::vector<std::pair<ContentType, std::string>> dirContent;
    for(auto &e: fs::directory_iterator(workingDir)) {
        ContentType t;
        auto name = e.path().filename().generic_string();
        switch (e.status().type()) {
        case fs::file_type::regular:
            t = FILE; break;
        case fs::file_type::directory:
            t = DIRECTORY; break;
        default:
            Log::GetLogger("ContentBrowser")->
                error(std::format("Unsupported file type: {}", name));
            assert(false);
        }
        dirContent.emplace_back(t, name);
    }
    return dirContent;
}

void
ContentBrowser::addFile(const std::string& filepath, const std::string directory, ContentType type) const {
    assert(fs::exists(filepath) && "provided path do not exist");
}

std::pair<bool, std::error_code> ContentBrowser::removeFile(const std::string& path) const {
    fs::path abspath = fs::absolute(path);
    assert(fs::exists(abspath) && "provided path do not exist");
    std::error_code er;
    bool removed = fs::remove(abspath, er);
    if (removed) { // remove resourse file entry, if exist
        
    }
    else {
        logger->warn(
            std::format("Can't remove file {}, err: {}", abspath.generic_string(), er.message()
        ));
    }
    return { removed, er };
}

void ContentBrowser::init() {
    logger = Log::GetLogger("ContentBrowser");
}

void ContentBrowser::checkContentFile() {

}

