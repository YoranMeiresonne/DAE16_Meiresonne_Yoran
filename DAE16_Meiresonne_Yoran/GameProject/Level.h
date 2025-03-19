#pragma once
#include <vector>
#include "Enemy.h" 
#include "Zen_Chan.h"

class Player;
class Level
{
public:
	Level(const int levelNr, std::vector<std::vector<Vector2f>> EnemyPos, std::vector<std::vector<std::vector<std::vector<Vector2f>>>>  Maps);
	~Level();

	void Draw();
	
	virtual void Update(float elapsedSec, Player* &player);
	void PlayerPosition(Player* &player);
	void SetMapInPosition(bool IsInPos);
	void EnemyIsHit(int EnemyIdx, bool PlayerHit = false);
	void ItemIsHit(int ItemIdx);
	void GetBubblePlayer(bool IsNot);
	void SetTranslMovement(float SavedMovement);
	void ResetGameTime();

	int GetNumEnemy();
	int GetLevel();
	int GetNumeItems();
	int GetPoints(int idx);
	int GetIntTypeBehaviorState(int EnemyIdx);
	
	float GetTranslMovement();
	virtual float GetGameTme();
	
	Rectf GetEnemyBoundingBox();
	Rectf GetEnemyBoundingBox(int EnemyIdx);

	Rectf GetItemBoundingBox();
	Rectf GetItemBoundingBox(int ItemIdx);

	bool GetBubbleEnemy(int EnemyIdx);
	bool GetMapInPosition();
	
private:
	int m_LevelNr;
	std::vector<std::vector<std::vector<std::vector<Vector2f>>>>   m_Maps;
	std::vector<std::vector<Vector2f>> m_EnemyPos;

	Texture* m_Tekst{ nullptr }; 
	Texture* m_LevelTekst{ nullptr };
	Texture* m_pPointSheet{ nullptr };
	
	Rectf m_PointBounds{};

	std::vector<Items*> m_Item;
	std::vector<Enemy*> Enemy;
	std::vector<Zen_Chan*> m_ZenEnemy;

	Player* m_Player;
	float m_TransMovement{};
	float m_AccuGameTime{0.f};

	bool m_MapInPosition{ true };
	bool m_MapLock{ false };
	bool m_IsBubblePlayer{ false };


};

