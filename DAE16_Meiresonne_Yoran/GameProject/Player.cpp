#include "pch.h"
#include "Player.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>
#include "CharacterController.h"
#include "Items.h"
#include "PointManager.h"
#include "Bubble.h"

Player::Player(const Vector2f& pos)
	:m_Position{pos}
	,m_accumulatedFrameTime{}
	,m_Point{}
	,m_accumulatedShootingTime{}
	
{
	m_Spritesheet = new Texture("PlayerSheet.png");
	
	m_pCharacterController = new CharacterController{
		Rectf{
			m_Position.x,
			m_Position.y,
			m_Spritesheet->GetWidth() / 9.f ,
			m_Spritesheet->GetHeight() / 6.f

		}
	};	

	m_Point = new PointManager(0);
	m_pBubble[0] = new Bubble(m_pCharacterController->GetBoundingBox().left, m_pCharacterController->GetBoundingBox().bottom, (int)m_BehaviorState);
}


Player::~Player() 
{
	delete m_Spritesheet; 
	m_Spritesheet = nullptr; 

}

void Player::Draw()
{
	const float scale{ 2.f };
	Rectf srcRect = GetCurrentFrameRect();
	int BigBubble{ 1 };
	if (m_BubblePlayer)
	{
		BigBubble = 2;
	}

	
	glPushMatrix();


	Rectf PlayerRect{ m_pCharacterController->GetBoundingBox().left,
	m_pCharacterController->GetBoundingBox().bottom,
	m_pCharacterController->GetBoundingBox().width * BigBubble,
	m_pCharacterController->GetBoundingBox().height * BigBubble };
	SetBoundingBox(PlayerRect);

	for (size_t i = 0; i < m_pBubble.size(); i++)
	{
		if (m_pBubble[i]->IsShot())
		{
			m_pBubble[i]->Draw();
			utils::DrawRect(m_pBubble[i]->GetBoundingRect());
		}
	}

	m_Spritesheet->Draw(PlayerRect,srcRect);
	glPopMatrix(); 
}

void Player::Update(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table) 
{
	m_accumulatedFrameTime += elapsedSec;
	m_accumulatedShootingTime += elapsedSec;
	m_accuCircleTime += elapsedSec;
	
	//Transport Player In Bubble
	if (m_BubblePlayer)
	{
		m_pCharacterController->SetCollisionOn(false);
	}
	else
	{
		m_pCharacterController->SetCollisionOn(true);
	}

	// StartScreen
	if (m_StartScreen)
	{
		StartScreen(elapsedSec, world_Vertices_Table);
	}

	if (m_InPosition == false || m_MapInPosition == false || m_BubblePlayer == true && !m_StartScreen )
	{
		ToPosition(elapsedSec, world_Vertices_Table, GetLevelPosition());
	}

	else if(m_InPosition && m_MapInPosition)
	{

		const float Gravity{ -150.f };
		float
			Horizontal_Movement{ 50.f };
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		m_Velocity.y += Gravity * elapsedSec;

		if (pStates[SDL_SCANCODE_LEFT] && !m_pCharacterController->HittingWall())
		{
			if (m_Velocity.x > 0.0f)
			{
				m_Velocity.x = 0.0f;
			}
			else
			{
			m_Velocity.x -= Horizontal_Movement * elapsedSec;
			m_BehaviorState = BehaviorState::left;
			m_IsWalking = true;
			}
		}
		else if (pStates[SDL_SCANCODE_RIGHT] && !m_pCharacterController->HittingWall())
		{
			if (m_Velocity.x < 0.0f)
			{
				m_Velocity.x = 0.0f;
			}
			else
			{
			m_Velocity.x += Horizontal_Movement * elapsedSec;
			m_BehaviorState = BehaviorState::right;
			m_IsWalking = true;
			}
		}
		else
		{
			m_Position.x = m_Position.x + m_Velocity.x;
			m_Velocity.x = 0.0f; // Stop movemenx²t when no keys are pressed
		}

		m_pCharacterController->Move(m_Velocity * elapsedSec, world_Vertices_Table);



		if (pStates[SDL_SCANCODE_SPACE])
		{
			if (m_ShootingCooldown == false)
			{
				m_CurrentFrame = 8;
				m_Attacking = true;
				m_pBubble.push_back(new Bubble(m_pCharacterController->GetBoundingBox().left, m_pCharacterController->GetBoundingBox().bottom, (int)m_BehaviorState));
				m_pBubble[m_pBubble.size() - 1]->BulletIsShot();
			}
			m_ShootingCooldown = true;
		}
		for (size_t i = 0; i < m_pBubble.size(); i++)
		{
			if (m_pBubble[i]->IsShot())
			{
				m_pBubble[i]->Update(elapsedSec); 
 			}
			if (m_pBubble[i]->GetBubbleTime() > 5.0f)
			{
				m_pBubble.erase(m_pBubble.begin() + i); 
			}
		}

		if (m_ShootingCooldown && m_accumulatedShootingTime >= 1.5f)
		{
			m_ShootingCooldown = false;
			m_accumulatedShootingTime = 0;
		}

		

		if (m_pCharacterController->IsOnGround())
		{
			m_Velocity.y = 0.0f; //Player Hit Ground
			if (pStates[SDL_SCANCODE_UP])
			{
		
				m_Velocity.y = 160.f;
				m_IsWalking = true;
				m_CurrentFrame = 6;
				m_Jumping = true;
				
			}
		}


		
	
	
		if (m_Attacking && m_accumulatedFrameTime >= 0.5f)
		{
			m_CurrentFrame++;
			m_accumulatedFrameTime = 0;
			m_Attacking = false;
		}

		if (m_Jumping && m_accumulatedFrameTime >= 0.5f)
		{
			m_CurrentFrame++;
			m_accumulatedFrameTime = 0;
			m_Jumping = false;
		}

		if (m_CurrentFrame < m_MaxFrames && m_accumulatedFrameTime >= 0.5f)
		{
			m_CurrentFrame++;
			m_accumulatedFrameTime = 0;
		}
		if (m_CurrentFrame >= m_MaxFrames && !m_Attacking)
		{
			m_CurrentFrame = 0;
		}
	}

	if (m_pCharacterController->GetBoundingBox().left < 33.f)
	{
		m_pCharacterController->SetBoundingBox(34.f, m_pCharacterController->GetBoundingBox().bottom);
	}
	if (m_pCharacterController->GetBoundingBox().left > 615.f)
	{
		m_pCharacterController->SetBoundingBox(614.f - m_pCharacterController->GetBoundingBox().width, m_pCharacterController->GetBoundingBox().bottom);
	}
}

void Player::PlayerInBubble()
{
	m_BubblePlayer = true;
	m_CurrentFrame = 2;
	if (!m_StartScreen)
	{
		m_InPosition = false;
	}
	
}


void Player::SetBoundingBox(Rectf& boundingbox)
{
	m_BoundingBox = boundingbox;
}

void Player::SetPosition(float x, float y)
{
	m_pCharacterController->SetBoundingBox(x, y);
}

void Player::SetStartScreen(bool OnOff)
{
	m_StartScreen = OnOff;
}

void Player::ToPosition(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table , Vector2f pos) 
{

	if (m_accumulatedFrameTime >= 0.5f) // Frame
	{
		if (m_InPosition && m_MapInPosition)
		{
			if (m_CurrentFrame == 2 || m_CurrentFrame == 1)
			{
				m_CurrentFrame = 0;
			}
			else
			{
				m_CurrentFrame = 1;
			}
		}
		else
		{
		
			if (m_CurrentFrame == 2)
			{
				m_CurrentFrame = 4;
			}
			else
			{
				m_CurrentFrame = 2;
			}
		}
		m_accumulatedFrameTime = 0;
	}


	if (m_InPosition == false && !m_StartScreen)
	{
		if (m_pCharacterController->GetBoundingBox().left < pos.x)
		{
			m_Velocity.x = 0;
		
			m_pCharacterController->GetBoundingBox().left == pos.x;
			
		}
		if (m_pCharacterController->GetBoundingBox().bottom < pos.y)
		{
			m_Velocity.y = 0.f;
			m_pCharacterController->GetBoundingBox().bottom == pos.y;
			
		}
		if (m_pCharacterController->GetBoundingBox().left > pos.x )
		{
			m_Velocity.x -= 50.f * elapsedSec;
		}
		if (m_pCharacterController->GetBoundingBox().bottom > pos.y )
		{
			m_Velocity.y -= 50.f * elapsedSec;
		}
			

		if (m_pCharacterController->GetBoundingBox().left < pos.x && m_pCharacterController->GetBoundingBox().bottom < pos.y)
		{
			m_InPosition = true;
		}
		m_pCharacterController->Move(m_Velocity * elapsedSec, world_Vertices_Table);
	}

	if (m_InPosition && m_MapInPosition)
	{
		m_BubblePlayer = false;
	}
}

void Player::SetLevelPosition(Vector2f pos)
{
	m_LevelPosition = pos;
}

Vector2f Player::GetLevelPosition()
{
	return m_LevelPosition;
}

void Player::StartScreen(float elapsedSec, const std::vector<std::vector<Vector2f>>& world_Vertices_Table)
{
	if (m_accumulatedFrameTime >= 0.5f)
	{
		if (m_CurrentFrame == 2)
		{
			m_CurrentFrame = 4;
		}
		else
		{
			m_CurrentFrame = 2;
		}
		m_accumulatedFrameTime = 0;
	}
	if (m_pCharacterController->GetBoundingBox().left > 220.f)
	{
		m_Velocity.x = 0;
		m_HoriCirleVelo = -20;
	}
	if (m_pCharacterController->GetBoundingBox().left < 110.f)
	{
		m_Velocity.x = 0;
		m_HoriCirleVelo = 20;
	}
	if (m_pCharacterController->GetBoundingBox().bottom > 200.f)
	{
		m_Velocity.y = 0;
		m_VeriCirleVelo = -20;
	}
	if (m_pCharacterController->GetBoundingBox().bottom < 100.f)
	{
		m_Velocity.y = 0;
		m_VeriCirleVelo = 20;
	}

	m_Velocity.x += m_HoriCirleVelo * elapsedSec;
	m_Velocity.y += m_VeriCirleVelo * elapsedSec;


	m_pCharacterController->Move(m_Velocity * elapsedSec, world_Vertices_Table);
}



const Rectf& Player::GetBoundingBox() const
{
	return m_BoundingBox;
}

const Rectf& Player::GetBubbleBoundingBox(int BubbleIdx)
{
	
	return m_pBubble[BubbleIdx]->GetBoundingRect();
	
}

Rectf Player::GetCurrentFrameRect()
{

	int BigBubble{ 1 };
	
	if (m_BehaviorState == BehaviorState::left)
	{
		m_Row = 0; 
		if (!m_IsAlive)
		{
			m_Row = 3;
			m_MaxFrames = 4;
		}
	}
	else if (m_BehaviorState == BehaviorState::right)
	{
		m_Row = 1;
		if (!m_IsAlive)
		{
			m_Row = 4;
			m_MaxFrames = 4;
		}
	}
	if (m_BubblePlayer)
	{
		BigBubble = 2;
		m_Row = 4;
		
	}

	Rectf sourceRect{
		m_CurrentFrame * m_Spritesheet->GetWidth() / 9.f ,
		m_Row * m_Spritesheet->GetHeight() / 6.f ,
		(m_Spritesheet->GetWidth() / 9.f) * BigBubble,
		(m_Spritesheet->GetHeight() / 6.f) * BigBubble
	};

	return sourceRect; 
	
}

bool Player::BubbleHitEnemy(Rectf& Enemy, int EnemyIntBehavior)
{
	if (EnemyIntBehavior != 2 && EnemyIntBehavior != 3)
	{
		for (int i = 0; i < m_pBubble.size(); i++)
		{
			if (utils::IsOverlapping(GetBubbleBoundingBox(i), Enemy))
			{
				m_pBubble[i]->~Bubble();
 				m_pBubble.erase(m_pBubble.begin() + i);
				return true;
			}

		}
	}
	return false;

}

bool Player::IsBubblePlayer()
{
	return m_BubblePlayer;
}

bool Player::IsPlayerAlive()
{
	return m_IsAlive;
}


void Player::GetMapInPosition(bool IsNot)
{
	m_MapInPosition = IsNot;
}

void Player::PlayerIsAlive(bool alive)
{
	m_IsAlive = alive;
}

void Player::SetVelocity(float x, float y)
{
	m_Velocity.x = x;
	m_Velocity.y = y;
}
