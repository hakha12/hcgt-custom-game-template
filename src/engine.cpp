#include "../include/engine.h"
#include "../scenes/scenes.h"

hcgt::Engine::Engine(): m_stateManager(std::make_shared<StateManager>()), m_isDone(false){
	Directory::Init();
	
	m_shared = std::make_shared<Shared>();

	m_shared->m_eventManager->AddCallback(StateType(0) ,"Window_close", &Engine::SetDone, this);

	std::shared_ptr<Scene_Template> scene_template = std::make_shared<Scene_Template>(m_stateManager, m_shared); 
	m_stateManager->AddState(StateType::TEMPLATE, scene_template);

	m_stateManager->SwitchTo(StateType::TEMPLATE);
}

hcgt::Engine::~Engine(){

}

void hcgt::Engine::HandleEvent(){
	m_shared->m_eventManager->HandleEvent(m_shared->m_window->GetScale());
}

void hcgt::Engine::Update(){
	m_shared->m_eventManager->Update(m_stateManager->GetCurrentState());
	m_stateManager->Update();
	m_shared->m_window->Update();
}

void hcgt::Engine::Draw(){
	m_shared->m_window->BeginDraw();
	m_stateManager->Draw();
	m_shared->m_window->EndDraw();
}

void hcgt::Engine::LateUpdate(){
	m_stateManager->LateUpdate();
}