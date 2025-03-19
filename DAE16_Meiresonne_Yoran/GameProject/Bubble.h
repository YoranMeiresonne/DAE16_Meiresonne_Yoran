#pragma once
#include "Texture.h"
#include <vector>

class Bubble
{
public:
	Bubble(float x, float y, int direction);
	~Bubble();

	void Draw();
	void Update(float elapsedsec);
	void BulletIsShot();
	Rectf GetCurrentFrameRect();
	Rectf GetBoundingRect();
	float GetBubbleTime();
	bool IsShot();


private:
	Texture* m_pBubbleSprite{ nullptr };

	int m_Direction{};
	int m_FrameNr{ 0 };
	Vector2f m_Position;

	Rectf m_BoundingBox;

	bool m_IsShot{ false };

	float m_Size{ 0.3f };
	float  m_accumulatedTime;
};

