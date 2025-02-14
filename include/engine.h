#pragma once

#include "shared.h"
#include "state.h"

namespace hcgt {

class Engine {
	public:
		Engine();
		~Engine();

		void HandleEvent();
		void Update();
		void Draw();
		void LateUpdate();

		void SetDone(std::shared_ptr<EventDetails> p_details){
			std::cout << "Done" << std::endl;
			m_isDone = true;}

		bool IsDone(){return m_isDone;}
	
	private:
		std::shared_ptr<Shared> m_shared;
		std::shared_ptr<StateManager> m_stateManager;

		bool m_isDone;
};

}