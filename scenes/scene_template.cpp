#include "scenes.h"

#include "../include/animate.h"


Scene_Template::Scene_Template(std::shared_ptr<hcgt::StateManager> p_stateManager, std::shared_ptr<hcgt::Shared> p_shared):State(p_stateManager, p_shared), scarfy(std::make_shared<hcgt::SpriteSheet>(p_shared->m_textureManager)){
	m_isTransparent = false;
}

Scene_Template::~Scene_Template(){
	
}

void Scene_Template::OnCreate(){
	scarfy->LoadSheet("scarfy.sheet");

}

void Scene_Template::OnDestroy(){
	
}

void Scene_Template::OnSwitchIn(){
	
}

void Scene_Template::OnSwitchOut(){
	
}

void Scene_Template::Update(){
	float dt = GetFrameTime();
	scarfy->SetAnimation("Default", true);
	Vector2 pos;
	pos.x = m_shared->m_window->GetInternalResolution().x/2;
	pos.y = m_shared->m_window->GetInternalResolution().y/2;
	scarfy->SetSpritePosition(pos);

	scarfy->Update(dt);
}

void Scene_Template::Draw(){
	ClearBackground(BLACK);
	scarfy->Draw();
	//DrawText("HEllo Window", m_shared->m_window->GetInternalResolution().x/2, m_shared->m_window->GetInternalResolution().y/2, 100, WHITE);
}

void Scene_Template::LateUpdate(){
	
}

