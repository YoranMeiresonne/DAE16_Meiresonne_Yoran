#include "pch.h"
#include "Enemy.h"
#include "Items.h"
#include "Zen_Chan.h"
#include "CharacterController.h"
#include "utils.h"
#include "Player.h"

Enemy::Enemy(const Vector2f& pos)
	:Enemy("",10,pos)
	
{

}

Enemy::Enemy(const std::string& spriteFile, int numFrames, const Vector2f& pos)
	:m_pSpriteSheet{new Texture(spriteFile)}
	,m_NumFrames{numFrames}
	,m_Position{pos}
	,m_Velocity{}
	,m_CurrentFrame{0}
	,m_accumulatedTime{} 
	,m_accumulatedBehaviorTime{}
	,m_Row{0}
	,m_MaxFrames{4}
	
{
	m_Item	= new Items{78, Vector2f(100.f, 100.f) };

	m_pCharacterController = new CharacterController{
	Rectf{
		m_Position.x,
		m_Position.y,
		m_pSpriteSheet->GetWidth() / 3.5f ,
		m_pSpriteSheet->GetHeight() / 3.5f

	}
	};
	m_Bounds = GetBoundingBox();
}

Enemy::~Enemy() 
{
	delete m_pSpriteSheet;
	m_pSpriteSheet = nullptr;
}

void Enemy::Draw() 
{
	Rectf srcRect = GetCurrentFrameRect();

	m_pSpriteSheet->Draw(GetBoundingBox(),srcRect);
}

void Enemy::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table, Player*& player)
{
	PlayerPosition(player);

	if (m_Bounds.left > 33.f && m_Bounds.left + m_Bounds.width < 615.f)
	{
		m_accumulatedBehaviorTime += elapsedSec;
		m_accumulatedTime += elapsedSec;

		const float Gravity{ -150.f };

		m_Velocity.y += Gravity * elapsedSec;

	

		m_pCharacterController->Move(m_Velocity * elapsedSec, world_Vertices_Table);
	
		if (IsOnGround())
		{
			m_Velocity.y = 0.0f;
			m_TimeFalling += elapsedSec;
		}

		

		if (IsOnGround() == false)
		{
			m_TimeFalling = 0.f;
		}

		if (m_pCharacterController->HittingWall())
		{
			m_Velocity.x = 0.0f;
		}

		if (m_CurrentFrame < m_MaxFrames && m_accumulatedTime >= 1.f)
		{
			m_CurrentFrame++;
			m_accumulatedTime = 0;
		}
		if (m_CurrentFrame >= m_MaxFrames)
		{
			m_CurrentFrame = 0;
		}
	}
	else
	{
		if (m_Bounds.left < 33.f)
		{
			m_Bounds.left = 36.f;
		}
		else
		{
			m_Bounds.left < 600.f;
		}
	}
}

void Enemy::SetGameTime(float Time)
{
	m_AccuGameTime = Time;
}

float Enemy::GetGameTime()
{
	return m_AccuGameTime;
}

bool Enemy::IsDead()
{
	return true;
}

bool Enemy::IsPlatformAbove()
{
	return m_pCharacterController->IsPlatformAbove();
}


Rectf Enemy::GetCurrentFrameRect()
{
	Rectf sourceRect{
		(float)m_pSpriteSheet->GetWidth(),
		(float)m_pSpriteSheet->GetHeight() ,
		(float)m_pSpriteSheet->GetWidth() / 16.f, 
		(float)m_pSpriteSheet->GetHeight() / 2.f
	};

	return sourceRect;
}

Vector2f Enemy::GetPlayerPosition()
{
	
	return Vector2f(m_Player->GetBoundingBox().left,m_Player->GetBoundingBox().bottom);
}


int Enemy::GetFrame() const
{
	return m_CurrentFrame ;
}

Texture* Enemy::GetSprite() const
{
	return m_pSpriteSheet; 
}

float Enemy::GetBehavioraccuTime()
{
	return m_accumulatedBehaviorTime;
}

float Enemy::GetTimeFalling()
{
	return m_TimeFalling;
}

void Enemy::SetBehaviorAccuTime(float Time)
{
	m_accumulatedBehaviorTime = Time;
}

bool Enemy::IsOnGround()
{
	return m_pCharacterController->IsOnGround();
}

bool Enemy::HittingWall()
{
	return m_pCharacterController->HittingWall();
}

bool Enemy::IsRealFalling()
{
	return m_pCharacterController->IsRealFalling();
}



void Enemy::PlayerPosition(Player* &player)
{
	m_Player = player;
}

int Enemy::GetRow() const
{
	return m_Row;
}

int Enemy::GetMaxFrames() const
{
	return m_MaxFrames;
}

void Enemy::SetRow(int row)
{
	m_Row = row;
}

void Enemy::SetMaxFrames(int Frames)
{
	m_MaxFrames = Frames;
}

void Enemy::SetCurrentFrame() 
{
	m_CurrentFrame = m_CurrentFrame ;
}

Vector2f Enemy::GetPosition()
{
	return Vector2f(m_pCharacterController->GetBoundingBox().left, m_pCharacterController->GetBoundingBox().bottom);
}

void Enemy::SetPosition(float x, float y)
{
	m_pCharacterController->SetBoundingBox(x, y);
}

void Enemy::SetVelocity(float x, float y)
{
	m_Velocity = Vector2f(x, y);
}

Vector2f Enemy::GetVelocity()
{
	return m_Velocity;
}

Rectf Enemy::GetBoundingBox()
{
	m_Bounds = Rectf(
		m_pCharacterController->GetBoundingBox().left,
		m_pCharacterController->GetBoundingBox().bottom,
		m_pSpriteSheet->GetWidth() / 3.5f,
		m_pSpriteSheet->GetHeight() / 3.5f
	);
	return m_Bounds;
}
