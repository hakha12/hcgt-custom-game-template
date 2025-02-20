#include "../include/animate.h"
#include "../include/directory.h"

hcgt::SpriteSheet::SpriteSheet(std::shared_ptr<TextureManager> p_textureManager):m_textureManager(p_textureManager), m_currentAnimation(nullptr), m_animationDirection(Direction::RIGHT){
	m_spriteScale = {1.0f, 1.0f};
}

hcgt::SpriteSheet::~SpriteSheet(){
	ReleaseSheet();
}

void hcgt::SpriteSheet::Update(const float& p_dt){
	if (m_currentAnimation) m_currentAnimation->Update(p_dt);
}

void hcgt::SpriteSheet::Draw(){
	Rectangle dest = {m_spritePos.x, m_spritePos.y, m_spriteSize.x * m_spriteScale.x, m_spriteSize.y * m_spriteScale.y};

	DrawTexturePro(*m_sprite, m_srcRec, dest, m_spriteorigin, 0.0f, WHITE);
}

void hcgt::SpriteSheet::CropSprite(Rectangle& p_rec){
	m_srcRec = p_rec;
}

std::shared_ptr<hcgt::Animation> hcgt::SpriteSheet::GetCurrentAnimation(){
	return m_currentAnimation;
}

const Vector2 hcgt::SpriteSheet::GetSpritePosition(){
	return m_spritePos;
}

const Vector2 hcgt::SpriteSheet::GetSpriteSize(){
	return m_spriteSize;
}

const hcgt::Direction hcgt::SpriteSheet::GetDirection(){
	return m_animationDirection;
}

void hcgt::SpriteSheet::SetSpritePosition(Vector2& p_pos){
	m_spritePos = p_pos;
}

void hcgt::SpriteSheet::SetSpriteSize(Vector2& p_size){
	m_spriteSize = p_size;
}

void hcgt::SpriteSheet::SetDirection(Direction& p_dir){
	if (m_animationDirection == p_dir) return;

	m_animationDirection = p_dir;

	if (m_currentAnimation) m_currentAnimation->_CropSprite();
}

bool hcgt::SpriteSheet::SetAnimation(const std::string& p_name, bool p_loop, bool p_play){
	std::unordered_map<std::string, std::shared_ptr<Animation>>::iterator it = m_animationList.find(p_name);

	if (it == m_animationList.end()) return false;

	if (it->second == m_currentAnimation) return false;

	if (m_currentAnimation) m_currentAnimation->Stop();

	m_currentAnimation = it->second;
	m_currentAnimation->SetLooping(p_loop);

	if (p_play) m_currentAnimation->Play();

	m_currentAnimation->_CropSprite();

	return true;
}

bool hcgt::SpriteSheet::LoadSheet(const std::string& p_file){
	std::string filePath = Directory::GetAssetPath() + "texture/" + p_file; // Need edit for better implementation for windows

	std::ifstream sheet(filePath);

	if (!sheet.is_open()){
		std::cerr << "Failed to load spritesheet from " << filePath << std::endl;

		return false;
	}

	ReleaseSheet();

	std::string line;

	while (std::getline(sheet, line)){
		if (line[0] == '|') continue;

		std::stringstream keystream(line);
		std::string type;

		keystream >> type;

		if (type == "Texture"){
			if (!m_textureName.empty()){
				std::cerr << "Duplicate texture entries in " << filePath << std::endl;

				continue;
			}

			std::string textureName;
			keystream >> textureName;

			if (!m_textureManager->RequireResource(textureName)){
				std::cerr << "Couldn't setup texture in " << filePath << std::endl;

				continue;
			}

			m_textureName = textureName;
			m_sprite = m_textureManager->GetResource(textureName);
		} else if (type == "Size"){
			keystream >> m_spriteSize.x >> m_spriteSize.y;

			SetSpriteSize(m_spriteSize);
		} else if (type == "Scale"){
			keystream >> m_spriteScale.x >> m_spriteScale.y;
		} else if (type == "Animation Type"){
			keystream >> m_animationType;
		} else if (type == "Animation"){
			std::string animationName;
			keystream >> animationName;

			if (m_animationList.find(animationName) != m_animationList.end()){
				std::cerr << "Duplicate animation in " << filePath << std::endl;

				continue;
			}

			std::shared_ptr<Animation> animation = std::make_shared<Animation>();

			animation->ReadIn(keystream);
			animation->SetSpriteSheet(shared_from_this());
			animation->SetName(animationName);
			animation->Reset();

			m_animationList.emplace(animationName, animation);

			if (!m_currentAnimation){
				m_currentAnimation = animation;
				m_currentAnimation->Play();
			}
		}
	}

	sheet.close();

	return true;
}

void hcgt::SpriteSheet::ReleaseSheet(){
	m_textureManager->ReleaseResource(m_textureName);

	m_currentAnimation = nullptr;

	m_animationList.clear();
}