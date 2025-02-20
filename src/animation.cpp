#include "../include/animate.h"

hcgt::Animation::Animation():m_currentFrame(0), m_startFrame(0), m_endFrame(0), m_frameRow(0), 
							m_frameTime(0.0f), m_elapsedTime(0.0f), m_frameActionStart(-1), m_frameActionEnd(-1), 
							m_isLooping(false), m_isPlaying(false){}

hcgt::Animation::~Animation(){}

void hcgt::Animation::Update(const float& p_dt){
	if (!m_isPlaying) return;

	m_elapsedTime += p_dt;

	if (m_elapsedTime < m_frameTime) return;

	_FrameStep();
	_CropSprite();

	m_elapsedTime = 0;
}

void hcgt::Animation::SetSpriteSheet(std::shared_ptr<SpriteSheet> p_sheet){
	m_spriteSheet = p_sheet;
}

void hcgt::Animation::SetFrame(unsigned int p_frame){
	if ((p_frame < m_startFrame && p_frame > m_endFrame) || (p_frame < m_endFrame && p_frame > m_startFrame)) return;

	m_currentFrame = p_frame;
}

void hcgt::Animation::SetStartFrame(unsigned int p_frame){
	m_startFrame = p_frame;
}

void hcgt::Animation::SetEndFrame(unsigned int p_frame){
	m_endFrame = p_frame;
}

void hcgt::Animation::SetFrameRow(unsigned int p_row){
	m_frameRow = p_row;
}

void hcgt::Animation::SetFrameTime(float p_time){
	m_frameTime = p_time;
}

void hcgt::Animation::SetLooping(bool p_value){
	m_isLooping = p_value;
}

void hcgt::Animation::SetName(const std::string& p_name){
	m_name = p_name;
}

std::shared_ptr<hcgt::SpriteSheet> hcgt::Animation::GetSpriteSheet(){
	return m_spriteSheet;
}

unsigned int hcgt::Animation::GetFrame(){
	return m_currentFrame;
}

unsigned int hcgt::Animation::GetStartFrame(){
	return m_startFrame;
}

unsigned int hcgt::Animation::GetEndFrame(){
	return m_endFrame;
}

unsigned int hcgt::Animation::GetFrameRow(){
	return m_frameRow;
}

int hcgt::Animation::GetActionStart(){
	return m_frameActionStart;
}

int hcgt::Animation::GetActionEnd(){
	return m_frameActionEnd;
}

float hcgt::Animation::GetFrameTime(){
	return m_frameTime;
}

float hcgt::Animation::GetElapsedTime(){
	return m_elapsedTime;
}

bool hcgt::Animation::IsLooping(){
	return m_isLooping;
}

bool hcgt::Animation::IsPlaying(){
	return m_isPlaying;
}

bool hcgt::Animation::IsInAction(){
	if (m_frameActionStart == -1 || m_frameActionEnd == -1) return true;

	return (m_currentFrame >= m_frameActionStart && m_currentFrame <= m_frameActionEnd);
}

std::string hcgt::Animation::GetName(){
	return m_name;
}

void hcgt::Animation::Play(){
	m_isPlaying = true;
}

void hcgt::Animation::Pause(){
	m_isPlaying = false;
}

void hcgt::Animation::Stop(){
	m_isPlaying = false;
	Reset();
}

void hcgt::Animation::Reset(){
	m_currentFrame = m_startFrame;
	m_elapsedTime = 0.0f;
	_CropSprite();
}

void hcgt::Animation::ReadIn(std::stringstream& p_stream){
	p_stream >> m_startFrame >> m_endFrame >> m_frameRow >> m_frameTime >> m_frameActionStart >> m_frameActionEnd;
}

void hcgt::Animation::_CropSprite(){
	Rectangle rec = {m_spriteSheet->GetSpriteSize().x * m_currentFrame, m_spriteSheet->GetSpriteSize().y * (m_frameRow + (short)m_spriteSheet->GetDirection()), 
					m_spriteSheet->GetSpriteSize().x, m_spriteSheet->GetSpriteSize().y};
	
	m_spriteSheet->CropSprite(rec);
}

void hcgt::Animation::_FrameStep(){
	if (m_startFrame < m_endFrame) ++m_currentFrame;
	else --m_currentFrame;

	if ((m_startFrame < m_endFrame && m_currentFrame > m_endFrame) || (m_startFrame > m_endFrame && m_currentFrame < m_endFrame)){
		if (m_isLooping) {
			m_currentFrame = m_startFrame;

			return;
		}

		m_currentFrame = m_endFrame;

		Pause();
	}
}
