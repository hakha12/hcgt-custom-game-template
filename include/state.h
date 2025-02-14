#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "shared.h"

namespace hcgt {

enum class StateType {
	TEMPLATE = 1,
	LOGO = TEMPLATE + 1,
	TITLE,
	LEVEL,
	PAUSED,
	GAME_OVER,
};

class StateManager;

class State {
	public:
		State(std::shared_ptr<StateManager> p_stateManager, std::shared_ptr<Shared> p_shared):m_stateManager(p_stateManager), m_shared(p_shared),m_isTransparent(false), m_isTranscendent(false){
		}

		virtual ~State(){}

		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;

		virtual void OnSwitchIn() = 0;
		virtual void OnSwitchOut() = 0;

		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void LateUpdate() = 0;

		void SetTransparent(bool p_value){m_isTransparent = p_value;}
		void SetTranscendent(bool p_value){m_isTranscendent = p_value;}

		const bool& IsTransparent(){return m_isTransparent;}
		const bool& IsTranscendent(){return m_isTranscendent;}

	protected:
		std::shared_ptr<StateManager> m_stateManager;
		std::shared_ptr<Shared> m_shared;
		bool m_isTransparent;
		bool m_isTranscendent;
};

class StateManager {
	public:
		StateManager();
		~StateManager();

		void Update();
		void Draw();
		void LateUpdate();


		void AddState(const StateType& p_type, std::shared_ptr<State> p_state);


		void SwitchTo(const StateType& p_type);
		void Remove(StateType& p_type);

		const StateType& GetCurrentState();
	
	private:
		std::unordered_map<StateType, std::shared_ptr<State>> m_stateList;
		std::vector<std::shared_ptr<State>> m_activeStateStack;		
		std::pair<StateType, std::shared_ptr<State>> m_currentState;
};

}