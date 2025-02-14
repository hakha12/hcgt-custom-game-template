#pragma once

#include <string>

#include <raylib.h>

namespace hcgt {

class Window {
	public:
		Window();
		Window(std::string p_title, int w, int h);
		~Window();

		const Vector2& GetInternalResolution();
		const float& GetScale();
		bool IsFocused();
		bool IsDone();

		void ToggleFullScreen();

		void Update();

		void BeginDraw();
		void EndDraw();

		void LateUpdate();
	
	private:
		std::string m_title;
		RenderTexture2D m_surface;
		Vector2 m_internalRes;
		Vector2 m_windowSize;
		float m_windowScale;
		bool m_isFullScreen;
		bool m_isFocused;
		bool m_isDone;

		void _Setup(const std::string& p_title, Vector2 p_size);
		void _Create();
		void _Scale();
		void _RenderSurface();
		void _Destroy();
};

}