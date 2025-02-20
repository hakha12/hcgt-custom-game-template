#pragma once

#include "../include/shared.h"
#include "../include/state.h"

#include "../include/animate.h"

class Scene_Template : public hcgt::State {
	public:
		Scene_Template(std::shared_ptr<hcgt::StateManager> p_stateManager, std::shared_ptr<hcgt::Shared> p_shared);

		~Scene_Template();

		void OnCreate() override;
		void OnDestroy() override;

		void OnSwitchIn() override;
		void OnSwitchOut() override;

		void Update() override;
		void Draw() override;
		void LateUpdate() override;
	
	private:
		std::shared_ptr<hcgt::SpriteSheet> scarfy;
};