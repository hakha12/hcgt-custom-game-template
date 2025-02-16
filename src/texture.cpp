#include "../include/resource.h"

std::shared_ptr<Texture2D> hcgt::TextureManager::Load(const std::string& p_path){
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();

	*texture = LoadTexture(p_path.c_str());

	return texture;
}