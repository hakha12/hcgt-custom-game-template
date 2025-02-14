#include "scenes.h"

Scene_Template::Scene_Template(std::shared_ptr<hcgt::StateManager> p_stateManager, std::shared_ptr<hcgt::Shared> p_shared):State(p_stateManager, p_shared){
	m_isTransparent = false;
}

Scene_Template::~Scene_Template(){
	
}

void Scene_Template::OnCreate(){

}

void Scene_Template::OnDestroy(){
	
}

void Scene_Template::OnSwitchIn(){
	
}

void Scene_Template::OnSwitchOut(){
	
}

void Scene_Template::Update(){
	
}

void Scene_Template::Draw(){
	ClearBackground(BLACK);

	DrawText("HEllo Window", m_shared->m_window->GetInternalResolution().x/2, m_shared->m_window->GetInternalResolution().y/2, 100, WHITE);
}

void Scene_Template::LateUpdate(){
	
}

