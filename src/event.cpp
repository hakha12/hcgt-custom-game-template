#include "../include/event.h"

hcgt::EventManager::EventManager(Directory& p_dir):m_hasFocus(true){
	_LoadBinding(p_dir.GetConfigPath() + "bindingKey.cfg");
}

hcgt::EventManager::~EventManager(){
	m_bindingList.clear();
	m_callbackList.clear();
}

void hcgt::EventManager::HandleEvent(const float& p_windowScale){
	_PollEvent();

	while (!m_eventQueue.empty()){
		Event event = m_eventQueue.front();
		m_eventQueue.pop();

		if (event.m_type == EventType::LOST_FOCUS){
			m_hasFocus = false;
		} else if (event.m_type == EventType::GAIN_FOCUS){
			m_hasFocus = true;
		} 

		_ProcessEvent(p_windowScale, event);
	}	
}

void hcgt::EventManager::Update(const StateType& p_currentState){
	if (!m_hasFocus) return;

	m_currentState = p_currentState;

	for (std::pair<const std::string, std::shared_ptr<Binding>> &b_itr : m_bindingList){
		std::shared_ptr<Binding> bind = b_itr.second;

		for (std::pair<EventType, EventInfo> &e_itr : bind->m_eventList){
			if (IsKeyDown(e_itr.second.m_code) && e_itr.first == EventType::Keyboard){
				bind->m_details.m_keyCode = e_itr.second.m_code;
				++(bind->m_eventCount);				
			} else if (e_itr.first == EventType::Mouse && IsMouseButtonDown(e_itr.second.m_code)) {
                bind->m_details.m_keyCode = e_itr.second.m_code;
                ++(bind->m_eventCount);
			} else {

			}
		}

		if (bind->m_eventList.size() == bind->m_eventCount){
			std::unordered_map<StateType, std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>>::iterator c_stateItr = m_callbackList.find(m_currentState);
			std::unordered_map<StateType, std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>>::iterator c_globalItr = m_callbackList.find(StateType(0));

			if (c_stateItr != m_callbackList.end()){
				std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>::iterator c_itr = c_stateItr->second.find(bind->m_name);

				if (c_itr != c_stateItr->second.end()){
					c_itr->second(std::make_shared<EventDetails>(bind->m_details));
				}
			}

			if (c_globalItr != m_callbackList.end()){
				std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>::iterator c_itr = c_globalItr->second.find(bind->m_name);

				if (c_itr != c_globalItr->second.end()){
					c_itr->second(std::make_shared<EventDetails>(bind->m_details));
				}
			}

		}

		bind->m_eventCount = 0;
		bind->m_details.Clear();
	}	
}

bool hcgt::EventManager::AddBinding(std::shared_ptr<Binding> p_binding){
	if (m_bindingList.find(p_binding->m_name) != m_bindingList.end()) return false;

	return m_bindingList.emplace(p_binding->m_name, p_binding).second;
}

bool hcgt::EventManager::RemoveBinding(const std::string& p_name){
	std::unordered_map<std::string, std::shared_ptr<Binding>>::iterator it = m_bindingList.find(p_name);

	if (it == m_bindingList.end()) return false;

	m_bindingList.erase(it);

	return true;
}

void hcgt::EventManager::SetFocus(bool p_value){
	m_hasFocus = p_value;
}

/*Vector2 hcgt::EventManager::GetVirtualMousePos(const float& p_windowScale){
 		Vector2 virtualMouse = { 0 };
        virtualMouse.x = (GetMousePosition().x - (GetScreenWidth() - (p_window.GetInternalResolution().x * p_window.GetScale())) * 0.5f) / p_window.GetScale();
        virtualMouse.y = (GetMousePosition().y - (GetScreenHeight() - (p_window.GetInternalResolution().y * p_window.GetScale())) * 0.5f) / p_window.GetScale();
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)p_window.GetInternalResolution().x, (float)p_window.GetInternalResolution().y });

		return virtualMouse;
}*/

// Private method

void hcgt::EventManager::_LoadBinding(const std::string& p_filePath){
	std::string separator = ":";

	std::ifstream bindingData;
	bindingData.open(p_filePath);

	if (!bindingData.is_open()){
		std::cout << "Failed to open " << p_filePath << std::endl;

		return;
	}

	std::string line;

	while (std::getline(bindingData, line)){
		std::stringstream keyStream(line);
		std::string callbackName;

		keyStream >> callbackName;

		std::shared_ptr<Binding> bind = std::make_shared<Binding>(callbackName);

		while (!keyStream.eof()){
			std::string keyVal;

			keyStream >> keyVal;

			int start = 0;
			int end = keyVal.find(separator);

			if (end == std::string::npos){
				bind.reset();

				break;
			}

			EventType type = EventType(std::stoi(keyVal.substr(start, end - start)));
			int code = std::stoi(keyVal.substr(end + separator.length(), keyVal.find(separator, end + separator.length())));
			EventInfo eventInfo;

			eventInfo.m_code = code;
			bind->BindEvent(type, eventInfo);
		}

		if (!AddBinding(bind)) bind.reset();
	}

	bindingData.close();
}

void hcgt::EventManager::_PollEvent(){
	for (int key = 0; key < 512; ++key) {
        if (IsKeyPressed(key)) {
            m_eventQueue.push({EventType::KEY_DOWN, key});
        } else if (IsKeyReleased(key)) {
            m_eventQueue.push({EventType::KEY_UP, key});
        }
    }

    for (int button = 0; button < 5; ++button) {
        if (IsMouseButtonPressed(button)) {
            m_eventQueue.push({EventType::MOUSE_BUTTON_DOWN, button});
        } else if (IsMouseButtonReleased(button)) {
            m_eventQueue.push({EventType::MOUSE_BUTTON_UP, button});
        }
    }

    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        m_eventQueue.push({EventType::MOUSE_WHEEL, (int)wheelMove});
    }

    static bool wasFocused = IsWindowFocused();
    bool isFocused = IsWindowFocused();
    if (isFocused != wasFocused) {
        wasFocused = isFocused;
        m_eventQueue.push({isFocused ? EventType::GAIN_FOCUS : EventType::LOST_FOCUS, 0});
    }

    if (IsWindowResized()) {
        m_eventQueue.push({EventType::WINDOW_RESIZED, 0});
    }

    if (WindowShouldClose()) {
        m_eventQueue.push({EventType::WINDOW_CLOSED, 0});
	}
}

void hcgt::EventManager::_ProcessEvent(const float& p_windowScale, Event& p_event){
	for (std::pair<const std::string, std::shared_ptr<Binding>> &b_itr : m_bindingList){
		std::shared_ptr<Binding> bind = b_itr.second;

		for (std::pair<EventType, EventInfo> &e_itr : bind->m_eventList){
			if (e_itr.first != p_event.m_type) continue;

			if (e_itr.first == EventType::KEY_DOWN || e_itr.first == EventType::KEY_UP){
				if (e_itr.second.m_code == p_event.m_code){
					bind->m_details.m_keyCode = e_itr.second.m_code;
					++(bind->m_eventCount);
			}
			} else if (e_itr.first == EventType::MOUSE_BUTTON_DOWN || e_itr.first == EventType::MOUSE_BUTTON_UP){
				if (e_itr.second.m_code == p_event.m_code){
					bind->m_details.m_mouse = GetMousePosition();
					++(bind->m_eventCount);
				}
			} else {
				if (e_itr.first == EventType::MOUSE_WHEEL){
					bind->m_details.m_mouseWheelDelta = GetMouseWheelMove();
				}

				if (e_itr.first == EventType::WINDOW_RESIZED){
					bind->m_details.m_size.x = GetScreenWidth();
					bind->m_details.m_size.y = GetScreenHeight();
				}

				if (e_itr.first == EventType::TEXT_ENTERED){
					bind->m_details.m_textEntered = GetCharPressed();
				}

				++(bind->m_eventCount);
			}
		}
	}
}