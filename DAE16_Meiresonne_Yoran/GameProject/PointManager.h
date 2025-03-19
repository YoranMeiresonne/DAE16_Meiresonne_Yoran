#pragma once
#include "Texture.h"

class PointManager
{
public:
	PointManager(int points = 0);
	~PointManager();

	void Draw(); 
	void GetPoints();
	void AddPoints(int AddAmount);
	void SetPoints(int Amount);
	void PrintPoints();
	void SetPosition(Vector2f& pos);

private:
	int m_Points{0};
	Vector2f m_Position;
	Texture* m_Score{ nullptr };
};

