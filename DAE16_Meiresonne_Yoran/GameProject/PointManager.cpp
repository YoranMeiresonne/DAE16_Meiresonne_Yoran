#include "pch.h"
#include "PointManager.h"
#include <iostream>
#include "Texture.h"

PointManager::PointManager(int points) 
	:m_Points{ points } 
	,m_Position{Vector2f(630.f / 4, 443.f)}
{
	m_Score = new Texture{ std::to_string(m_Points),"VCR_OSD_MONO_1.001.ttf",30,Color4f{1.f,1.f,1.f,1.f} };
}
PointManager::~PointManager()
{

}

void PointManager::Draw()
{
	GetPoints();
	m_Score->Draw(m_Position);
}

void PointManager::GetPoints()
{
	m_Score = new Texture{ std::to_string(m_Points),"VCR_OSD_MONO_1.001.ttf",30,Color4f{1.f,1.f,1.f,1.f} };
}

void PointManager::AddPoints(int AddAmount)
{
	m_Points += AddAmount;
}
void PointManager::SetPoints(int Amount)
{
	m_Points = Amount;
}
void PointManager::PrintPoints()
{
	std::cout << "Points: " << m_Points << std::endl;
}

void PointManager::SetPosition(Vector2f& pos)
{
	m_Position = pos;
}
