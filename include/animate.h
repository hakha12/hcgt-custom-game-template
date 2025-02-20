#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "resource.h"

namespace hcgt {

enum class Direction {
	RIGHT = 0,
	LEFT,
};

class SpriteSheet;

class Animation {
		friend class SpriteSheet;
	public:
		Animation();
		~Animation();

		void Update(const float& p_dt);

		void SetSpriteSheet(std::shared_ptr<SpriteSheet> p_sheet);

		void SetFrame(unsigned int p_frame);
		void SetStartFrame(unsigned int p_frame);
		void SetEndFrame(unsigned int p_frame);
		void SetFrameRow(unsigned int p_row);
		void SetFrameTime(float p_time);

		void SetLooping(bool p_value);
		void SetName(const std::string& m_name);

		std::shared_ptr<SpriteSheet> GetSpriteSheet();
		unsigned int GetFrame();
		unsigned int GetStartFrame();
		unsigned int GetEndFrame();
		unsigned int GetFrameRow();
		int GetActionStart();
		int GetActionEnd();
		float GetFrameTime();
		float GetElapsedTime();
		bool IsLooping();
		bool IsPlaying();
		bool IsInAction();
		std::string GetName();

		void Play();
		void Pause();
		void Stop();
		void Reset();

		void ReadIn(std::stringstream& p_stream);
	protected:
		std::shared_ptr<SpriteSheet> m_spriteSheet;
		std::string m_name;

		unsigned int m_currentFrame;
		unsigned int m_startFrame;
		unsigned int m_endFrame;
		unsigned int m_frameRow;

		float m_frameTime;
		float m_elapsedTime;

		int m_frameActionStart;
		int m_frameActionEnd;

		bool m_isLooping;
		bool m_isPlaying;

		void _FrameStep();
		void _CropSprite();
		
};

class SpriteSheet : public std::enable_shared_from_this<SpriteSheet> {
	public:
		SpriteSheet(std::shared_ptr<TextureManager> p_textureManager);
		~SpriteSheet();

		void CropSprite(Rectangle& p_rec);
		const Vector2 GetSpritePosition();
		const Vector2 GetSpriteSize();

		void SetSpritePosition(Vector2& p_pos);
		void SetSpriteSize(Vector2& p_size);

		void SetDirection(Direction& p_dir);
		const Direction GetDirection();

		bool LoadSheet(const std::string& p_file);
		void ReleaseSheet();

		std::shared_ptr<Animation> GetCurrentAnimation();
		bool SetAnimation(const std::string& p_name, bool p_loop = false, bool p_play = false);

		void Update(const float& p_dt);
		void Draw();
	
	private:
		std::shared_ptr<Texture2D> m_sprite;
		std::string m_textureName;
		Vector2 m_spritePos;
		Vector2 m_spriteSize;
		Vector2 m_spriteScale;
		Vector2 m_spriteorigin;
		Rectangle m_srcRec;

		std::unordered_map<std::string, std::shared_ptr<Animation>> m_animationList;
		std::shared_ptr<Animation> m_currentAnimation;
		Direction m_animationDirection;
		std::string m_animationType;

		std::shared_ptr<TextureManager> m_textureManager;
};

}