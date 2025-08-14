#pragma once
#include "Texture.h"
#include "Items.h"
#include "CharacterController.h"


class player;
class Enemy
{
public:
	Enemy(const Vector2f& pos);
	Enemy(const std::string& spriteFile, int numFrames, const Vector2f& pos);
	~Enemy();

	virtual void Draw();
	void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table, Player*& player);
	void SetGameTime(float Time);

	float GetGameTime();
	
	bool virtual IsDead();
	bool IsPlatformAbove();

	virtual Rectf GetBoundingBox();
	virtual Rectf GetCurrentFrameRect();

	Vector2f GetPlayerPosition();

protected:

	int GetFrame() const;
	int GetRow() const;
	int GetMaxFrames() const;
	
	void SetRow(int row);
	void SetMaxFrames(int Frames);
	void SetCurrentFrame();
	void SetPosition(float x, float y);
	void SetVelocity(float x, float y);
	void SetBehaviorAccuTime(float Time);

	Vector2f GetPosition();
	Vector2f GetVelocity();

	Texture* GetSprite() const;

	float GetBehavioraccuTime();
	float GetTimeFalling();

	bool IsOnGround();
	bool HittingWall();
	bool IsRealFalling();
	
private:
	Texture* m_pSpriteSheet;

	int m_NumFrames;
	int m_CurrentFrame;
	int m_Row;
	int m_MaxFrames;

	float  m_accumulatedTime;
	float m_accumulatedBehaviorTime;
	float m_AccuGameTime{ 0.f };
	float m_TimeFalling{ 0.f };

	Rectf m_Bounds;

	Vector2f m_Position;
	Vector2f m_Velocity;

	Items* m_Item{ nullptr };

	CharacterController* m_pCharacterController{ nullptr };

	Player* m_Player;

	void PlayerPosition(Player*& player);
	
};

