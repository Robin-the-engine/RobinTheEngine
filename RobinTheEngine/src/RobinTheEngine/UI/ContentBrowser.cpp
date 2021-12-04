#include "rtepch.h"
#include "ContentBrowser.h"
#include "yaml-cpp/yaml.h"
#include <format>
#include <commdlg.h>
#include "../YAMLHelper.h"

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
            logger->error(std::format("Unsupported file type: {}", name));
            assert(false);
        }
        dirContent.emplace_back(t, name);
    }
    return dirContent;
}

void
ContentBrowser::addFile(const std::string& filepath, const std::string directory, ContentType type) const {
    if(filepath.empty()) {
        logger->info("Empty path provided to addFile. No file was selected?");
        return;
    }
    if(!fs::exists(filepath)) {
        logger->error(std::format("path: {} do not exist!", filepath));
    }
    fs::copy_file(filepath, directory);
    // TODO: Add to yaml
}

std::pair<bool, std::error_code> ContentBrowser::removeFile(const std::string& path) const {
    fs::path abspath = fs::absolute(path);
    std::error_code er;
    // TODO: remove resource file entry, if exist
    bool removed = fs::remove(abspath, er);
    if (!removed) {
        logger->warn(
            std::format("Can't remove file {}, err: {}", abspath.generic_string(), er.message())
        );
    }
    return { removed, er };
}

std::string ContentBrowser::getNextDir(const std::string& current, const std::string& next) {
    return fs::path(current).append(next).generic_string();
}

void ContentBrowser::init() {
    logger = Log::GetLogger("ContentBrowser");
}

void ContentBrowser::checkContentFile() {
    std::unordered_map<std::string, std::string> content;
    std::unordered_map<std::string, MeshDesc> meshContent;
    YamlHelper::ReadResourceFile(contentFile, content, meshContent);
    for(const auto &e: content) {
        
    }
}

const std::string ContentBrowserGUI::openFileDialog() {
    OPENFILENAMEA ofn;
    constexpr const size_t max_name_length = 512;
    char filename[max_name_length] = {'\0'};
    size_t sz = sizeof(OPENFILENAME);
    ZeroMemory(&ofn, sz);
    ofn.lStructSize = sz;
    ofn.lpstrFile = filename;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.nMaxFile = max_name_length;
    auto cwd = fs::current_path();
    // changes cwd, so we have to restore it
    bool success = GetOpenFileNameA(&ofn);
    fs::current_path(cwd);
    return ofn.lpstrFile;
}
