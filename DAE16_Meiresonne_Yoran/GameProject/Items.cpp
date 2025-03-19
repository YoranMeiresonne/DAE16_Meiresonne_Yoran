#include "pch.h"
#include "Items.h"
#include "Texture.h"
#include "utils.h"
#include "Player.h"
#include <iostream>

Items::Items(int numFrames, const Vector2f& pos)
	:m_pItemsSheet{ new Texture("Items.png")}
	,m_NumFrames{numFrames}
	,m_Position{pos}
	
{
	
}

Items::~Items() 
{
	m_pItemsSheet = nullptr;
	delete m_pItemsSheet;

}

void Items::Draw()
{
	Rectf srcRect = GetCurrentFrameRect();

	m_pItemsSheet->Draw(GetBoundingBox(),srcRect);
}

void Items::Update(const Rectf& PlayerBoundingBox)
{
	Rectf dstRect(
		m_Position.x,
		m_Position.y,
		m_pItemsSheet->GetWidth() / 20.f,
		m_pItemsSheet->GetHeight() / 4.f
	);

}

Rectf Items::GetCurrentFrameRect()
{
	m_CurrenFrame = m_NumFrames % 20;
	m_Row = m_NumFrames / 20;

	Rectf sourceRect{
		m_CurrenFrame * m_pItemsSheet->GetWidth() /20.f,
		m_Row * m_pItemsSheet->GetHeight() / 4.f ,
		(float)m_pItemsSheet->GetWidth() / 20.f,
		(float)m_pItemsSheet->GetHeight() / 4.f
	};

	return sourceRect;
}



int Items::GetPoints()
{
	return (m_CurrenFrame + 1) * 10;
}

void Items::SetFrame(int Frame)
{
	if (Frame > 20)
	{
		Frame = Frame % 20;
		m_Row++;
	}
	m_CurrenFrame = Frame;
	
}

void Items::SetPosition(const Vector2f& pos)
{
	m_Position = pos;
}

Rectf Items::GetBoundingBox()
{
	m_Bounds = Rectf(
		m_Position.x,
		m_Position.y,
		m_pItemsSheet->GetWidth() / 20.f,
		m_pItemsSheet->GetHeight() / 4.f
	);
	return m_Bounds;
}

