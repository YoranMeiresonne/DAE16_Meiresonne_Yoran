#include "pch.h"
#include "Camera.h"

Camera::Camera(float ScreenWidth, float ScreenHeight)
	:m_ScreenWidth{ ScreenWidth }
	, m_ScreenHeight{ ScreenHeight }
{
}

void Camera::Aim(float levelW, float levelH, const Vector2f& trackCenter)
{
	Vector2f Camera;

	Camera.x = trackCenter.x - m_ScreenWidth / 2;
	Camera.y = trackCenter.y - m_ScreenHeight / 2;

	if (Camera.x + m_ScreenWidth > levelW)
	{
		Camera.x = levelW - m_ScreenWidth;
	}
	if (Camera.x < 0)
	{
		Camera.x = 0.f;
	}
	if (Camera.y + m_ScreenHeight > levelH)
	{
		Camera.y = levelH - m_ScreenHeight;
	}
	if (Camera.y < 0 )
	{
		Camera.y = 0.f;
	}
	

	glPushMatrix();
	glTranslatef(-Camera.x, -Camera.y, 0.0f);

}

Vector2f Camera::Reset()
{
	glPopMatrix();
	return Vector2f{ 1.f,1.f };
}