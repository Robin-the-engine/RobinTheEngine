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
        std::format("Set content file to: {}", fs::absolute(contentFile).string())
    );
    scanContentFile();
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
ContentBrowser::addFile(const std::string& filepath, const std::string directory, ContentType type) {
    if(filepath.empty()) {
        logger->info("Empty path provided to addFile. No file was selected?");
        return;
    }
    if(!fs::exists(filepath)) {
        logger->error(std::format("path: {} do not exist!", filepath));
        return;
    }
    fs::path filename = fs::path(filepath).filename();
    fs::path copyDst = fs::path(directory).append(filename.string());
    fs::copy_file(filepath, copyDst, fs::copy_options::update_existing);
    const std::string& key = filename.replace_extension().string();
    const std::string value = copyDst.string();
    if(type == ContentType::TEXTURE) {
        textures[key] = value;
    }
    else if (type == ContentType::SHADER) {
        shaders[key] = value;
    }
    else if (type == ContentType::MESH) {
        MeshDesc d;
        d.layout = -1;
        d.path = value;
        d.key = key;
        meshes[key] = d;
    }
    updateContentFile(type);
}

std::pair<bool, std::error_code> ContentBrowser::removeFile(const std::string& path) {
    fs::path abspath = fs::absolute(path);
    std::error_code er;
    ContentType type = removeAndGetType(path);
    updateContentFile(type);
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

void ContentBrowser::scanContentFile() {
    scanContentGroup(YamlHelper::TEXTURE_GROUP, textures);
    scanContentGroup(YamlHelper::SHADER_GROUP, shaders);
    scanMeshGroup(YamlHelper::MESH_GROUP, meshes);
}

void ContentBrowser::scanContentGroup(
    const std::string& groupName,
    std::unordered_map<std::string, std::string>& group
) {
    std::unordered_map<std::string, std::string> tmpContent;
    YamlHelper::ReadGroup(contentFile, groupName, tmpContent);
    for (const auto& e : tmpContent) {
        if (!fs::exists(e.second)) {
            logger->warn(std::format("Key: {}, file: {} do not exist!", e.first, e.second));
        }
        else {
            group.insert(e);
        }
    }

}

void ContentBrowser::scanMeshGroup(
    const std::string& groupName,
    std::unordered_map<std::string, MeshDesc>& group
) {
    std::unordered_map<std::string, MeshDesc> tmpMeshContent;
    YamlHelper::ReadMeshes(contentFile, groupName, tmpMeshContent);
    for (const auto& e : tmpMeshContent) {
        if (!fs::exists(e.second.path)) {
            logger->warn(std::format("Key: {}, file: {} do not exist!", e.first, e.second.path));
        }
        else {
            meshes.insert(e);
        }
    }
}

void ContentBrowser::updateContentFile(ContentType type) const {
    if (type == ContentType::MESH || type == ContentType::SHADER || type == ContentType::TEXTURE) {
        YamlHelper::UpdateResourceFile(contentFile, shaders, textures, meshes);
    }
}


ContentBrowser::ContentType ContentBrowser::removeAndGetType(const std::string filepath) {

    auto removeKey = [&filepath = filepath](std::unordered_map<std::string, std::string>& group) {
        std::string keyToRemove;
        for (const auto& e : group) {
            if (fs::equivalent(e.second, filepath)) {
                keyToRemove = e.first;
                break;
            }
        }
        if(!keyToRemove.empty()) {
            group.erase(keyToRemove);
            return true;
        }
        return false;
    };

    if(removeKey(shaders)) {
        return SHADER;
    }
    if(removeKey(textures)) {
        return TEXTURE;
    }

    std::string keyToRemove;
    for (const auto& e : meshes) {
        if (fs::equivalent(e.second.path, filepath)) {
            keyToRemove = e.first;
            break;
        }
    }
    if (!keyToRemove.empty()) {
        meshes.erase(keyToRemove);
        return MESH;
    }

    return UNKNOWNN;
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
