#pragma once
#include "Texture.h"

class PointManager;
class Player;
class Items
{
public:

	Items(int numFrames, const Vector2f& pos);
	~Items();

	void Draw();
	void Update(const Rectf& PlayerBoundingBox);
	int GetPoints();
	void SetFrame(int Frame);
	void SetPosition(const Vector2f& pos);
	Rectf GetCurrentFrameRect();
	Rectf GetBoundingBox();



private:
	Texture* m_pItemsSheet{ nullptr };
	Texture* m_pPoint{ nullptr };

	int m_NumFrames;
	int m_CurrenFrame{ 0 };
	int m_Row{ 0 };
	

	Vector2f m_Position;
	Rectf m_Bounds;
	Player* m_Player{ nullptr };
};

