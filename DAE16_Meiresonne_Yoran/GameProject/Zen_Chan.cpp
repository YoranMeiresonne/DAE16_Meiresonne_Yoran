#include "pch.h"
#include "Zen_Chan.h"
#include "Items.h"
#include "Player.h"
#include "iostream"
#include "utils.h"


Zen_Chan::Zen_Chan(const Vector2f& pos)
	:Enemy("ZenChanEnemySheet.png", 32, pos)
	, m_BubbleFloating{ 10.f }
	, m_IsAngry{ false }
	, m_IsBubble{false}
	,m_IsDefeated{false}
	,m_IsDead{false}
{	
	
}
Zen_Chan::~Zen_Chan()
{
	Enemy::~Enemy();
}

void Zen_Chan::Draw()
{
	Enemy::Draw();
}

void Zen_Chan::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table, Player*& player)
{
	if (Enemy::GetBoundingBox().left < 33.f)
	{
		Enemy::SetPosition(33.5f, Enemy::GetBoundingBox().bottom);
	}

	if (Enemy::GetBoundingBox().left + Enemy::GetBoundingBox().width > 615.f)
	{ 
		Enemy::SetPosition(613.5f - Enemy::GetBoundingBox().width, Enemy::GetBoundingBox().bottom);
	} 
	if (!Enemy::IsOnGround())
	{
		SetVelocity(0.f, Enemy::GetVelocity().y);
		m_Stilfalling = true;
	}
	else
	{
		m_Stilfalling = false;
	}

	if (Enemy::GetGameTime() > 30.f && GetBehaviorState() != BehaviorState::bubble && GetBehaviorState() != BehaviorState::defeated)
	{
		SetBehaviorState(BehaviorState::angry);
		Enemy::SetGameTime(0.f);
	}
	m_AccMove += elapsedSec;
	Enemy::Update(elapsedSec, world_Vertices_Table,player);
	Behavior();
	
}

void Zen_Chan::Behavior()
{
	switch (m_BehaviorState)
	{
	case Zen_Chan::BehaviorState::normal:
		m_IsAngry = false;
		Move(); 
		break;
	case Zen_Chan::BehaviorState::angry:
		Angry();
		break;
	case Zen_Chan::BehaviorState::bubble:
		Bubble();
		break;
	case Zen_Chan::BehaviorState::defeated:
		Defeated();
		break;
	default:
		break;
	}

}

void Zen_Chan::Move()
{
	int Multipl(1);
	
	if (m_IsAngry)
	{
		Multipl = 2;
	}
	else
	{
		Multipl = 1;
	}

	if (m_AccMove > 3.f)
	{
		m_Direction = rand() % 2;
		m_AccMove = 0.f;
	}

	if (Enemy::GetPlayerPosition().y > (Enemy::GetPosition().y + 0.8f) && Enemy::IsPlatformAbove() && m_Stilfalling == false) // Player is above them
	{
		VerticalJump();
	}



	if (Enemy::HittingWall() == false )
	{
		
		if (m_Direction == 0 )
		{
			SetVelocity(-30.f * Multipl, Enemy::GetVelocity().y); // Move left
			
		}
		else if(m_Direction == 1)
		{
			SetVelocity(30.f * Multipl, Enemy::GetVelocity().y); // Move right
		}
	}
	
	if (Enemy::HittingWall())
	{
		SetVelocity(0.f, Enemy::GetVelocity().y);

	}
}

void Zen_Chan::VerticalJump()
{
	SetVelocity(0, 150.f);
	
}

void Zen_Chan::HorizontalJump()
{
	SetVelocity(Enemy::GetVelocity().x, 50.f);
}

void Zen_Chan::Angry()
{
	Enemy::SetRow(1);
	m_IsAngry = true;
	Move();
}

void Zen_Chan::Defeated()
{
	const float Xmove{ 40.f };

	Enemy::SetRow(2);
	Enemy::SetMaxFrames(8);
	if (Enemy::GetBoundingBox().left < 33.f)
	{
		Enemy::SetPosition(35.f, Enemy::GetBoundingBox().bottom);
	}

	if (Enemy::GetBoundingBox().left > 615.f)
	{
		Enemy::SetPosition(610.f, Enemy::GetBoundingBox().bottom);
	}
	if (Enemy::GetPosition().x < 669.f / 2.f)
	{
		if (Enemy::GetBehavioraccuTime() < 3.f)
		{
			SetVelocity(Xmove, m_BubbleFloating);
		}
		if (Enemy::GetBehavioraccuTime() > 3.f)
		{
			SetVelocity(-Xmove - 10.f, Enemy::GetVelocity().y);
		}
	}
	else
	{
		if (Enemy::GetBehavioraccuTime() < 3.f)
		{
			SetVelocity(-Xmove, m_BubbleFloating);
		}
		if (Enemy::GetBehavioraccuTime() > 3.f)
		{
			SetVelocity(Xmove + 10.f, Enemy::GetVelocity().y);
		}
	}
	
	if (Enemy::IsOnGround())
	{
		SetVelocity(0.f, Enemy::GetVelocity().y);
		m_IsDead = Enemy::IsDead();
	}

}

bool Zen_Chan::IsDead()
{
	return m_IsDead;
}

void Zen_Chan::Bubble()
{
	Enemy::SetRow(5);
	Enemy::SetMaxFrames(3);
	

	SetVelocity(0.f, m_BubbleFloating);

	if (Enemy::GetPosition().y >= m_BehaviorChangePosition.y + 50.f && m_BubbleFloating > 0)
	{
		m_BubbleFloating = -m_BubbleFloating;
	}
	if (Enemy::GetPosition().y <= m_BehaviorChangePosition.y && m_BubbleFloating < 0)
	{
		m_BubbleFloating = -m_BubbleFloating;
	}
}

Vector2f Zen_Chan::DeadPosition()
{
	return Enemy::GetPosition();
}



Rectf Zen_Chan::GetCurrentFrameRect()
{
	Rectf sourceRect{
		Enemy::GetFrame() * GetSprite()->GetWidth() / 8.f,
		Enemy::GetRow() * GetSprite()->GetHeight() / 8.f ,
		GetSprite()->GetWidth() / 8.f,
		GetSprite()->GetHeight() / 8.f
	};
	return sourceRect;
}

Rectf Zen_Chan::GetBoundingBox()
{
	return Enemy::GetBoundingBox();
}

void Zen_Chan::SetBehaviorState(const BehaviorState& Behavior)
{
	SetBehaviorAccuTime(0.f);
	m_BehaviorChangePosition = GetPosition();
	m_BehaviorState = Behavior;
}

Zen_Chan::BehaviorState Zen_Chan::GetBehaviorState()
{
	return m_BehaviorState; 
}

bool Zen_Chan::IsBubble()
{
	return m_IsBubble;
}
