#include "../include/directory.h"

hcgt::Directory::Directory(){
	std::string separator;
	std::string filePath = ".";
	
	#if defined(WINDOWS)
		separator = "\\";
	#else
		separator = "/";
	#endif
	#if defined(VS_CODE)
		filePath = filePath + ".";
	#endif	
	m_dir = filePath + separator;
	m_assetDir = m_dir + "assets" + separator;
	m_configDir = m_dir + "config" + separator;
}

const std::string& hcgt::Directory::Get(){
	return m_dir;
}

const std::string& hcgt::Directory::GetAssetPath(){
	return m_assetDir;
}

const std::string& hcgt::Directory::GetConfigPath(){
	return m_configDir;
}