#pragma once
#include "Vector2f.h"

class Camera
{
public:
	Camera(float ScreenWidth, float ScreenHeight);

	void Aim(float levelW, float levelH, const Vector2f& trackCenter); 
	Vector2f Reset();

private:
	float m_ScreenWidth;
	float m_ScreenHeight;
};
