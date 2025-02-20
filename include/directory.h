#pragma once

#include <string>

namespace hcgt {

class Directory {
public:
    static void Init();

    static std::string Get();
    static std::string GetAssetPath();
    static std::string GetConfigPath();

private:
    static std::string& Dir();
    static std::string& AssetDir();
    static std::string& ConfigDir();

    static std::string separator;
    static std::string filePath;
};

} // namespace hcgt
