#include "../include/window.h"

#include "raymath.h" 

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

// Public method
hcgt::Window::Window(){
	_Setup("HRT RAYLIB TEMPLATE", (Vector2){1280, 720});
}

hcgt::Window::Window(std::string p_title, int w, int h){
	_Setup(p_title, (Vector2){w, h});
}

hcgt::Window::~Window(){
	_Destroy();
}

void hcgt::Window::Update(){
	_Scale();
}

void hcgt::Window::BeginDraw(){
	BeginTextureMode(m_surface);
	ClearBackground(WHITE);
}

void hcgt::Window::EndDraw(){
	EndTextureMode();

	_RenderSurface();
}

void hcgt::Window::LateUpdate(){

}

const Vector2& hcgt::Window::GetInternalResolution(){
	return m_internalRes;
}

const float& hcgt::Window::GetScale(){
	return m_windowScale;
}

bool hcgt::Window::IsFocused(){
	return m_isFocused;
}

bool hcgt::Window::IsDone(){
	return m_isDone;
}

void hcgt::Window::ToggleFullScreen(){
	m_isFullScreen = !m_isFullScreen;
}

// Private method

void hcgt::Window::_Setup(const std::string& p_title, Vector2 p_size){
	m_title = p_title;
	m_windowSize = p_size;
	m_windowScale = 1.0f;
	m_isFocused = true;
	m_isFullScreen = false;
	m_isDone = false;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

	m_internalRes = (Vector2){1920, 1080};

	_Create();
}

void hcgt::Window::_Create(){
	InitWindow(m_windowSize.x, m_windowSize.y, m_title.c_str());
	SetWindowMinSize(320, 240);

	m_surface = LoadRenderTexture(m_internalRes.x, m_internalRes.y);
	SetTextureFilter(m_surface.texture, TEXTURE_FILTER_BILINEAR);
}

void hcgt::Window::_Scale(){
	m_windowScale = MIN((float)GetScreenWidth()/m_internalRes.x, (float)GetScreenHeight()/m_internalRes.y);
}

void hcgt::Window::_RenderSurface(){
	BeginDrawing();
	ClearBackground(WHITE);

	DrawTexturePro(m_surface.texture, (Rectangle){ 0.0f, 0.0f, (float)m_surface.texture.width, (float)-m_surface.texture.height },
                  (Rectangle){ (GetScreenWidth() - ((float)m_internalRes.x*m_windowScale))*0.5f, (GetScreenHeight() - ((float)m_internalRes.y*m_windowScale))*0.5f,
                  (float)m_internalRes.x*m_windowScale, (float)m_internalRes.y*m_windowScale}, (Vector2){ 0, 0 }, 0.0f, WHITE);
    
	EndDrawing();
}

void hcgt::Window::_Destroy(){
	CloseWindow();
}
