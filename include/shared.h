#pragma once

#include "directory.h"
#include "event.h"
#include "window.h"

namespace hcgt {

struct Shared {
	Shared():m_window(std::make_shared<Window>()), m_dir(std::make_shared<Directory>()), m_eventManager(std::make_shared<EventManager>(*m_dir)){}

	std::shared_ptr<Window> m_window;
	std::shared_ptr<Directory> m_dir;
	std::shared_ptr<EventManager> m_eventManager;	
};

}