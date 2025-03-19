#pragma once
#include "Enemy.h"

class Items; 
class Zen_Chan : public Enemy
{
public:
	enum class BehaviorState
	{
		normal,
		angry,
		bubble,
		defeated
	};
	Zen_Chan(const Vector2f& pos);
	~Zen_Chan();

	virtual void Draw();
	virtual void Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table, Player*& player);
	void Behavior();
	void Move();
	void VerticalJump();
	void HorizontalJump();
	void Angry();
	void Bubble();
	void Defeated();
	void SetBehaviorState(const BehaviorState& Behavior);

	bool IsDead();
	bool IsBubble();

	Vector2f DeadPosition();

	virtual Rectf GetCurrentFrameRect() override;
	virtual Rectf GetBoundingBox() override;
	
	BehaviorState GetBehaviorState();
	

protected:
	

private:
	Vector2f m_MovementSpeed{};
	Vector2f m_BehaviorChangePosition{};

	float m_BubbleFloating{};
	float m_AccTimeAlive{};
	float m_AccMove{ 0.f };

	int m_Direction{ 0 };

	bool m_IsAngry{};
	bool m_IsBubble{};
	bool m_IsDefeated{};
	bool m_IsDead{};
	bool m_Stilfalling{ false };


	Zen_Chan::BehaviorState m_BehaviorState{ BehaviorState::normal };
};

