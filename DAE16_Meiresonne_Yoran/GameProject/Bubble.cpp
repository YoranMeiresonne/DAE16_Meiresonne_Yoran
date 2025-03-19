#include "pch.h"
#include "Bubble.h"
#include "utils.h"



Bubble::Bubble(float x, float y,int direction)
	:m_Position{ Vector2f{x,y} }
	,m_Direction{direction}
	, m_accumulatedTime{0.f}
{
	m_pBubbleSprite = new Texture("Bubbles.png");
	m_BoundingBox = GetBoundingRect();
}
Bubble::~Bubble()
{
	delete m_pBubbleSprite;
	m_pBubbleSprite = nullptr;
}

void Bubble::Draw()
{
	Rectf sourceRect = GetCurrentFrameRect(); 
	Rectf dstRect = GetBoundingRect();
	m_pBubbleSprite->Draw(dstRect, sourceRect);
	utils::SetColor(Color4f(1.f, 0.5f, 1.f, 1.f));
	
}
void Bubble::Update(float elapsedsec)
{
	m_accumulatedTime += elapsedsec;

	if (m_Size < 0.75f)
	{
		if (m_Direction == 0 || m_Direction == 2 )
		{
			m_Position.x -= 200.f * elapsedsec;
		}
		else
		{
			m_Position.x += 200.f * elapsedsec;
		}

		m_Size += elapsedsec / 2.f;
	}
	else
	{
		m_FrameNr = 1; // Bubble Becomes Bell
	}


}

void Bubble::BulletIsShot()
{
	m_IsShot = true;
}

Rectf Bubble::GetCurrentFrameRect()
{

	Rectf sourceRect{
		m_FrameNr * m_pBubbleSprite->GetWidth() / 2.f ,
		m_pBubbleSprite->GetHeight() ,
		m_pBubbleSprite->GetWidth() / 2.f ,
		m_pBubbleSprite->GetHeight()
	};

	return sourceRect;

}

Rectf Bubble::GetBoundingRect()
{

	Rectf dstRect{ m_Position.x, m_Position.y, m_pBubbleSprite->GetWidth() / 2.f * m_Size, m_pBubbleSprite->GetHeight()* m_Size };
	m_BoundingBox = dstRect;
	return m_BoundingBox;

}

float Bubble::GetBubbleTime()
{
	return m_accumulatedTime;
}

bool Bubble::IsShot()
{
	return m_IsShot;
}

