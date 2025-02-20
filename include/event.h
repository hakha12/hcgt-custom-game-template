#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <queue>
#include <functional>
#include <unordered_map>

#include <raylib.h>
#include <raymath.h>

#include "directory.h"

namespace hcgt {

enum class EventType {
    KEY_DOWN = 0,
    KEY_UP,
    MOUSE_BUTTON_DOWN,
    MOUSE_BUTTON_UP,
    MOUSE_WHEEL,
    WINDOW_RESIZED,
	WINDOW_CLOSED,
    GAIN_FOCUS,
    LOST_FOCUS,
    MOUSE_ENTER,
    MOUSE_LEFT,
   	TEXT_ENTERED,
    Keyboard = TEXT_ENTERED + 1,
	Mouse, 
	Joystick
};

enum class StateType;

struct Event {
	EventType m_type;
	int m_code;

	Event(EventType t, int c) : m_type(t), m_code(c) {}
};

struct EventInfo {
    EventInfo() { m_code = 0; }
    EventInfo(int l_event) { m_code = l_event; }
    union {
        int m_code;
    };
};

struct EventDetails {
    EventDetails(const std::string& p_bindName):m_name(p_bindName){
        Clear();
    }

    std::string m_name;

    Vector2 m_size;
    unsigned int m_textEntered;
    Vector2 m_mouse;
    int m_mouseWheelDelta;
    int m_keyCode;

    void Clear() {
        m_size = { 0, 0 };
        m_textEntered = 0;
        m_mouse = { 0, 0 };
        m_mouseWheelDelta = 0;
        m_keyCode = -1;
    }
};

struct Binding {
    Binding(const std::string& p_name) : m_name(p_name), m_eventCount(0), m_details(p_name){}

    void BindEvent(EventType p_type, EventInfo p_info = EventInfo()) {
        m_eventList.emplace_back(p_type, p_info);
    }

    std::vector<std::pair<EventType, EventInfo>> m_eventList;
    std::string m_name;
    int m_eventCount;

    EventDetails m_details;
};

class EventManager {
	public:
		EventManager();
		~EventManager();

		void HandleEvent(const float& p_windowScale);
		void Update(const StateType& p_currentState);

		bool AddBinding(std::shared_ptr<Binding> p_binding);
		bool RemoveBinding(const std::string& p_name);

		void SetFocus(bool p_value);

		template<class T>
		bool AddCallback(StateType p_type, const std::string& p_name, void(T::*p_function)(std::shared_ptr<EventDetails>), T* p_instance){
			std::unordered_map<StateType, std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>>::iterator itr;
			
			itr = m_callbackList.emplace(p_type, std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>()).first;

			std::function<void(std::shared_ptr<EventDetails>)> temp = std::bind(p_function, p_instance, std::placeholders::_1);

			return itr->second.emplace(p_name, temp).second;
		}

		bool RemoveCallback(StateType p_type, const std::string& p_name){
			std::unordered_map<StateType, std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>>::iterator it = m_callbackList.find(p_type);

			if (it == m_callbackList.end()) return false;

			std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>::iterator it2 = it->second.find(p_name);

			if (it2 == it->second.end()) return false;

			it->second.erase(p_name);

			return true;
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Binding>> m_bindingList;
		std::unordered_map<StateType, std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>> m_callbackList;

		std::queue<Event> m_eventQueue;

		bool m_hasFocus;

		StateType m_currentState;

		void _LoadBinding(const std::string& p_filePath);
		void _PollEvent();
		void _ProcessEvent(const float& p_windowScale, Event& p_event);
};
}