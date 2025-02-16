#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <unordered_map>

#include <raylib.h>

namespace hcgt {

template<typename Derived, typename T>
class ResourceManager {
	public:
		ResourceManager(const std::string& p_filePath){
			_LoadPath(p_filePath);
		}

		virtual ~ResourceManager(){
			PurgeResources();
		}

		std::shared_ptr<T> GetResource(const std::string& p_id){
			typename std::unordered_map<std::string, std::shared_ptr<T>>::iterator it = m_resourceList.find(p_id);

			return (it != m_resourceList.end()) ? it->second : nullptr;
		}

		const std::string GetPath(const std::string& p_id){
			std::unordered_map<std::string, std::string>::iterator path = m_pathList.find(p_id);

			return (path != m_pathList.end()) ? path->second : "";
		}

		bool RequireResource(const std::string& p_id){
			if (m_resourceList.find(p_id) != m_resourceList.end()) return true;

			std::unordered_map<std::string, std::string>::iterator path = m_pathList.find(p_id);

			if (path == m_pathList.end()) return false;

			std::shared_ptr<T> resourceData = Load(path->second);

			if (!resourceData) return false;

			m_resourceList.emplace(p_id, std::move(resourceData));
			
			return true;
		}

		void ReleaseResource(const std::string& p_id){
			m_resourceList.erase(p_id);
		}

		void PurgeResources(){
			m_resourceList.clear();
		}

	protected:
		std::shared_ptr<T> Load(const std::string& p_path){
			return static_cast<Derived*>(this)->Load(p_path);
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<T>> m_resourceList;
		std::unordered_map<std::string, std::string> m_pathList;

		bool _Unload(const std::string& p_id){
			typename std::unordered_map<std::string, std::shared_ptr<T>>::iterator it = m_resourceList.find(p_id);

			if (it == m_resourceList.end()) return false;

			m_resourceList.erase(it);

			return true;
		}

		void _LoadPath(const std::string& p_path){
			std::ifstream pathDir;

			pathDir.open(p_path);

			if (!pathDir.is_open()){
				std::cerr << "Failed to open path-to-resource-file.cfg ." << std::endl;

				return;
			}

			std::string line;

			while (std::getline(pathDir, line)){
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;

				keystream >> pathName;
				keystream >> path;

				m_pathList.emplace(pathName, path);
			}

			pathDir.close();
		}
};

class TextureManager : public ResourceManager<TextureManager, Texture2D> {
	public:
		TextureManager(const std::string& p_filePath): ResourceManager(p_filePath){}

		std::shared_ptr<Texture2D> Load(const std::string& p_path);
};

}