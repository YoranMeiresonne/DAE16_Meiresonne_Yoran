#pragma once
#include <vector>
class Items;
class CharacterController
{
public:
	CharacterController(const Rectf& StartBoundingBox);
	~CharacterController();

	void Move(const Vector2f& deltaMovement, const std::vector<std::vector<Vector2f>>& worldVerticesTable);
	void HitItem(const std::vector<Items*>& items);
	void SetCollisionOn(bool OnOff);
	void SetBoundingBox(float x, float y);
	void SetPosition(const Vector2f& position);

	bool IsPlatformAbove();
	bool IsRealFalling();
	bool Collision() const;
	bool IsOnGround() const;
	bool HitRoof() const;
	bool HittingWall() const;

	const Rectf& GetBoundingBox() const;

	

private:
	bool m_CollisionOn{ true };
	bool m_RealFalling{ false };
	bool m_ItIsOnGround{ false };
	bool m_HitRoof{ false };
	bool m_HittingWall{ false };
	bool m_PlatformAbove{ false };
	Rectf m_Bounds{};
	
};
