#pragma once
#include "Texture.h"
#include <vector>
#include "PointManager.h"


class PointManager;
class CharacterController;


class Bubble;

class Player
{
public:
	enum class BehaviorState
	{
		left,
		right,
	
	};

	Player(const Vector2f& pos);
	~Player(); 

	void Draw();
	void Update(float elapsedSec,
				const std::vector<std::vector<Vector2f>>& worldVerticesTable);

	void PlayerInBubble();
	void SetBoundingBox(Rectf& boundingbox);
	void SetPosition(float x, float y);
	void SetStartScreen(bool OnOff);
	void ToPosition(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table, Vector2f pos );
	void SetLevelPosition(Vector2f pos);
	void StartScreen(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table);
	void GetMapInPosition(bool IsNot);
	void PlayerIsAlive(bool alive);
	void SetVelocity(float x, float y);

	Vector2f GetLevelPosition();
	const Rectf& GetBoundingBox() const;
	const Rectf& GetBubbleBoundingBox(int BubbleIdx);

	bool BubbleHitEnemy(Rectf& Enemy, int EnemyIntBehavior);
	bool IsBubblePlayer();
	bool IsPlayerAlive();
	


private:
	Player::BehaviorState m_BehaviorState{ BehaviorState::left};
	Texture* m_Spritesheet{ nullptr };

	Vector2f m_Position;
	Vector2f m_Velocity{0.0f,0.0f};
	Vector2f m_LevelPosition{};
	

	int m_CurrentFrame{ 0 };
	int m_MaxFrames{ 5 };
	int m_Row{ 0 };

	bool m_IsWalking{ false };
	bool m_Attacking{ false };
	bool m_Jumping{ false };
	bool m_BubblePlayer{ false };
	bool m_IsAlive{ true };
	bool m_Shoot{ false };
	bool m_ShootingCooldown{ false };
	bool m_StartScreen{ true };
	bool m_OutSideBound{ false };
	bool m_aBubbleExist{ false };
	bool m_MapInPosition{ false };

	bool m_InPosition{ false };

	float  m_accumulatedFrameTime;
	float  m_accumulatedShootingTime;
	float m_accuCircleTime;
	float m_Angle{ 1.f };
	float m_HoriCirleVelo{ 20.f };
	float m_VeriCirleVelo{ 20.f };

	Rectf GetCurrentFrameRect();
	Rectf m_BoundingBox;
	CharacterController* m_pCharacterController{ nullptr }; 

	std::vector<Bubble*>m_pBubble{ nullptr };

	PointManager* m_Point{};


};

