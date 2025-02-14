#include "../include/state.h"

hcgt::StateManager::StateManager():m_stateList(0){
	m_currentState.first = StateType(0); // change when we had another state
}

hcgt::StateManager::~StateManager(){
	for (std::pair<const StateType, std::shared_ptr<State>> &itr : m_stateList){
		itr.second->OnDestroy();
	}

	m_stateList.clear();
}

void hcgt::StateManager::Update(){
	if (m_activeStateStack.empty()) return;

	for (std::vector<std::shared_ptr<State>>::reverse_iterator it = m_activeStateStack.rbegin(); it != m_activeStateStack.rend(); ++it){
		(*it)->Update();

		if (!(*it)->IsTranscendent()) break;
	}
}

void hcgt::StateManager::Draw(){
	if (m_activeStateStack.empty()) return;

	int firstOpaqueState = -1;

	for (int i = 0; i < m_activeStateStack.size(); ++i){
		if (!m_activeStateStack.at(i)->IsTransparent()){
			firstOpaqueState = i;

			break;
		}
	}

	for (int i = firstOpaqueState; i < m_activeStateStack.size(); ++i){
		m_activeStateStack.at(i)->Draw();
	}
}

void hcgt::StateManager::LateUpdate(){
	if (m_activeStateStack.empty()) return;

}

void hcgt::StateManager::AddState(const StateType& p_type, std::shared_ptr<State> p_state){
	std::pair<std::unordered_map<StateType, std::shared_ptr<State>>::iterator, bool> inserted = m_stateList.insert(std::make_pair(p_type, p_state));

	inserted.first->second->OnCreate();
}

void hcgt::StateManager::SwitchTo(const StateType& p_type){
	std::unordered_map<StateType, std::shared_ptr<State>>::iterator  it = m_stateList.find(p_type);

	if (it == m_stateList.end()) return;

	m_currentState.first = p_type;

	std::shared_ptr<State> newState = it->second;

	if (!m_activeStateStack.empty()){
		std::shared_ptr<State> currentTopState = m_activeStateStack.back();

		if (!currentTopState->IsTransparent()) {
			currentTopState->OnSwitchOut();

			m_activeStateStack.pop_back();
		}
	}

	newState->OnSwitchIn();

	m_activeStateStack.push_back(newState);
}

void hcgt::StateManager::Remove(StateType& p_type){

}

const hcgt::StateType& hcgt::StateManager::GetCurrentState(){
	return m_currentState.first;
}