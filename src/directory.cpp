#include "../include/directory.h"
#include <iostream>

namespace hcgt {

// Define static member variables
std::string hcgt::Directory::separator = "/";
std::string hcgt::Directory::filePath = ".";

std::string& hcgt::Directory::Dir() {
    static std::string m_dir = filePath + separator;
    return m_dir;
}

std::string& hcgt::Directory::AssetDir() {
    static std::string m_assetDir = Dir() + "assets" + separator;
    return m_assetDir;
}

std::string& hcgt::Directory::ConfigDir() {
    static std::string m_configDir = Dir() + "config" + separator;
    return m_configDir;
}

void hcgt::Directory::Init() {
    #if defined(WINDOWS)
        separator = "\\";
    #else
        separator = "/";
    #endif

    #if defined(VS_CODE)
        filePath = "..";
    #else
        filePath = ".";
    #endif

    // Force re-initialization
    Dir() = filePath + separator;

    std::cout << "Initialize Directory: " << Dir() << std::endl;
}

std::string hcgt::Directory::Get() {
    return Dir();
}

std::string hcgt::Directory::GetAssetPath() {
    return AssetDir();
}

std::string hcgt::Directory::GetConfigPath() {
    return ConfigDir();
}

} // namespace hcgt
