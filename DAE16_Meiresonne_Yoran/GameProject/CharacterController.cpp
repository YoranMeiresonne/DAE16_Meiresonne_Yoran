#include "pch.h"
#include "CharacterController.h"
#include "utils.h"
#include "Items.h"
#include <iostream>

CharacterController::CharacterController(const Rectf& StartBoundingBox)
	:m_Bounds{ StartBoundingBox }
{
}

CharacterController::~CharacterController()
{

}

void CharacterController::SetPosition(const Vector2f& position)
{
	m_Bounds.left = position.x;
	m_Bounds.bottom = position.y;
}

void CharacterController::Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& worldVerticesTable)
{
	if (m_Bounds.left > 33.f && m_Bounds.left + m_Bounds.width < 615.f)
	{
		m_ItIsOnGround = false; 
		m_HittingWall = false;

		if (!m_RealFalling)
		{
			m_Bounds.left += deltaMovement.x;
		}
	
		m_Bounds.bottom += deltaMovement.y;

		if (Collision())
		{
			utils::HitInfo hitInfo;

			const Vector2f
				rayLeftStart{ m_Bounds.left - 2.f,m_Bounds.bottom },
				rayLeftEnd{ rayLeftStart.x,rayLeftStart.y + m_Bounds.height },
				rayRightStart{ m_Bounds.left + m_Bounds.width + 2.f ,m_Bounds.bottom },
				rayRightEnd{ rayRightStart.x,rayRightStart.y + m_Bounds.height };


			for (size_t idx{ 0 }; idx < worldVerticesTable.size(); ++idx)
			{
				const std::vector<Vector2f>& platform = worldVerticesTable[idx];


				if (utils::Raycast(platform, rayLeftStart, rayLeftEnd, hitInfo)
					|| utils::Raycast(platform, rayRightStart, rayRightEnd, hitInfo)
					)
				{
					if (deltaMovement.y < 0) // Falling
					{
						m_ItIsOnGround = true;
						m_Bounds.bottom = hitInfo.intersectPoint.y;
					}

					break;
				}

				if (utils::Raycast(platform, Vector2f(rayLeftStart.x + 5.f, rayLeftStart.y), Vector2f(rayLeftEnd.x + 5.f,rayLeftEnd.y + 30.f), hitInfo)
					|| utils::Raycast(platform, Vector2f(rayRightStart.x - 5.f, rayRightStart.y), Vector2f(rayRightEnd.x - 5.f,rayRightEnd.y + 30.f), hitInfo))
				{
					m_PlatformAbove = true;
					break;
				}
				else
				{
					m_PlatformAbove = false;
				}

				
			}



			bool hitLeftWall = false;
			bool hitRightWall = false;

			const Vector2f
				rayBottomLeft{ m_Bounds.left, m_Bounds.bottom },
				rayBottomRight{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom },
				rayTopLeft{ m_Bounds.left, m_Bounds.bottom + m_Bounds.height },
				rayTopRight{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + m_Bounds.height };

			for (size_t i = 0; i < worldVerticesTable.size(); ++i)
			{
				const std::vector<Vector2f>& platform = worldVerticesTable[i];

				if (utils::Raycast(platform, rayBottomLeft, rayTopLeft, hitInfo)) // Left collision
				{
				
					hitLeftWall = true;
					
					break;
				}
				else if (utils::Raycast(platform, rayBottomRight, rayTopRight, hitInfo)) // Right collision
				{
				
					hitRightWall = true;
					
					break;
				}
			}
	

			if (hitLeftWall && deltaMovement.x < 0) // Block leftward movement
			{
				m_HittingWall = true;
			
				m_Bounds.left -= deltaMovement.x; // Undo left movement
			}
			else if (hitRightWall && deltaMovement.x > 0) // Block rightward movement
			{
				m_HittingWall = true;
				m_Bounds.left -= deltaMovement.x; // Undo right movement
			}
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
void CharacterController::HitItem(const std::vector<Items*>& items) 
{
	for (size_t i = 0; i < items.size(); i++)
	{
		if (utils::IsOverlapping(m_Bounds, items[i]->GetCurrentFrameRect())) 
		{
		
		}
		
	}
}
void CharacterController::SetCollisionOn(bool OnOff)
{
	m_CollisionOn = OnOff;
}
void CharacterController::SetBoundingBox(float x, float y)
{
	m_Bounds.left = x;
	m_Bounds.bottom = y;
}

bool CharacterController::IsPlatformAbove()
{
	return m_PlatformAbove;
}

bool CharacterController::IsRealFalling()
{
	return m_RealFalling;
}

bool CharacterController::Collision() const
{
	return m_CollisionOn;
}
bool CharacterController::IsOnGround() const
{
	return m_ItIsOnGround;
}


bool CharacterController::HitRoof() const
{
	return m_HitRoof;
}

bool CharacterController::HittingWall() const
{
	return m_HittingWall;
}

const Rectf& CharacterController::GetBoundingBox() const
{
	
	return Rectf{ m_Bounds.left ,m_Bounds.bottom ,m_Bounds.width * 2.f ,m_Bounds.height * 2.f };
}

