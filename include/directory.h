#pragma once

#include <string>

namespace hcgt {

class Directory {
	public:
		Directory();

		const std::string& Get();
		const std::string& GetAssetPath();
		const std::string& GetConfigPath();
	
	private:
		std::string m_dir;
		std::string m_assetDir;
		std::string m_configDir;
};

}