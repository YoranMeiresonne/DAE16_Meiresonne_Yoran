#include "pch.h"
#include "Level.h"
#include <iostream>
#include "utils.h"
#include "Items.h"
#include "Player.h"

Level::Level(const int levelNr, std::vector<std::vector<Vector2f>> EnemyPos, std::vector<std::vector<std::vector<std::vector<Vector2f>>>>  Maps)
	:m_LevelNr{levelNr}
	,m_Maps {Maps} 
	,m_EnemyPos{EnemyPos} 
	,m_ZenEnemy{}


	
{ 
	for (size_t i = 0; i < EnemyPos.size(); i++)
	{
		m_ZenEnemy.push_back(new Zen_Chan(EnemyPos[i][0]));
	}


	m_Tekst = new Texture{ "NOW IT IS THE BEGINNING OF A FANTASTIC STORY! LET US MAKE A JOURNEY TO THE CAVE OF MONSTERS! GOOD LUCK!",
							"VCR_OSD_MONO_1.001.ttf",
							30,
							Color4f{1.f,1.f,1.f,1.f} 
						 };

}

Level::~Level()
{
	for (size_t i = 0; i < m_ZenEnemy.size(); i++)
	{
		delete m_ZenEnemy[i];
	}
}

void Level::Draw() 
{
	if (m_MapInPosition && !m_IsBubblePlayer)
	{
		for (int i = 0; i < m_ZenEnemy.size(); i++)
		{
			m_ZenEnemy[i]->Draw();
		}
		for (int i = 0; i < m_Item.size(); i++)
		{
			m_Item[i]->Draw();
		}

	}

}

void Level::Update(float elapsedSec, Player* &player)
{
	PlayerPosition(player);
	float MapPos{0.f};
	float IsDone{0.f};


	if (m_MapInPosition && !m_IsBubblePlayer)
	{
		
		
		for (int i = 0; i < m_ZenEnemy.size(); i++)
		{
			if (m_ZenEnemy[i]->GetGameTime() == 0.f)
			{
				m_AccuGameTime = 0.f;
			}
			m_AccuGameTime += elapsedSec;
			if (m_ZenEnemy[i]->IsDead())
			{
				m_Item.push_back(new Items(rand() % 10, m_ZenEnemy[i]->DeadPosition()));
				m_ZenEnemy.erase(m_ZenEnemy.begin() + i);
				m_EnemyPos.resize(m_EnemyPos.size() - 1);
				m_ZenEnemy.shrink_to_fit();
			}
			else
			{
				m_ZenEnemy[i]->SetGameTime(m_AccuGameTime);
				m_ZenEnemy[i]->Update(elapsedSec, m_Maps[0][m_LevelNr], player);
			}
		}
	}
	

}

int Level::GetNumEnemy()
{
	return (int)m_EnemyPos.size();
}

int Level::GetLevel()
{
	return m_LevelNr;
}

int Level::GetNumeItems()
{
	return (int)m_Item.size();
}

int Level::GetPoints(int idx)
{
	return m_Item[idx]->GetPoints();
}

float Level::GetTranslMovement()
{
	return m_TransMovement;
}

void Level::PlayerPosition(Player* &player) 
{
	m_Player = player ; 
	
}

void Level::SetMapInPosition(bool IsInPos)
{
	m_MapInPosition = IsInPos; 
}

int Level::GetIntTypeBehaviorState(int EnemyIdx)
{
	return (int)m_ZenEnemy[EnemyIdx]->GetBehaviorState(); 
}

float Level::GetGameTme()
{
	return m_AccuGameTime;
}

Rectf Level::GetEnemyBoundingBox()
{
	for (int i = 0; i < m_ZenEnemy.size(); i++)
	{
		return m_ZenEnemy[i]->GetBoundingBox();
	}
}

Rectf Level::GetEnemyBoundingBox(int EnemyIdx)
{
	return m_ZenEnemy[EnemyIdx]->GetBoundingBox();
}

Rectf Level::GetItemBoundingBox()
{
	for (int i = 0; i < m_Item.size(); i++)
	{
		return m_Item[i]->GetBoundingBox();
	}
}

Rectf Level::GetItemBoundingBox(int ItemIdx)
{
	return m_Item[ItemIdx]->GetBoundingBox(); 
}

bool Level::GetBubbleEnemy(int EnemyIdx)
{
	if (m_ZenEnemy[EnemyIdx]->GetBehaviorState() == Zen_Chan::BehaviorState::bubble || m_ZenEnemy[EnemyIdx]->GetBehaviorState() == Zen_Chan::BehaviorState::angry)
	{
		return true;
	}
	return false;
}

bool Level::GetMapInPosition()
{
	return m_MapInPosition;
}

void Level::EnemyIsHit(int EnemyIdx,bool PlayerHit)
{	
	if (m_ZenEnemy[EnemyIdx]->GetBehaviorState() == Zen_Chan::BehaviorState::bubble && PlayerHit)
	{
		m_ZenEnemy[EnemyIdx]->SetBehaviorState(Zen_Chan::BehaviorState::defeated);
	}
	else if (m_ZenEnemy[EnemyIdx]->GetBehaviorState() != Zen_Chan::BehaviorState::defeated)
	{
		m_ZenEnemy[EnemyIdx]->SetBehaviorState(Zen_Chan::BehaviorState::bubble);
	}
}

void Level::ItemIsHit(int ItemIdx)
{
	m_Item.erase(m_Item.begin() + ItemIdx);
	m_Item.shrink_to_fit();
}

void Level::GetBubblePlayer(bool IsNot)
{
	m_IsBubblePlayer = IsNot;
}

void Level::SetTranslMovement(float SavedMovement)
{
	m_TransMovement = SavedMovement;
}

void Level::ResetGameTime()
{
	m_AccuGameTime = 0.f;
}

