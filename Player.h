// COMP710 JESSE
#include "vector2.h"
#ifndef __Player_H__
#define __Player_H__
// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
class Vector2;
class b2Vec2;
// Class declaration:
class Player
{
	// Member methods:
public:
	Player();
	~Player();
	Vector2 Position();
	float GetShipAngle();
	bool CanDamage();
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);
	float getSpeed();
	float GetRadius();
	virtual void DebugDraw();
	void aiming();

	void drift();
	static Player& GetInstance();
	void Fling();
	void SetPosition(Vector2 position);
	void SetPosition(b2Vec2 position);
	static void DestroyInstance();

protected:

private:
	Player(const Player& Player);
	Player& operator=(const Player& Player);

	// Member data:
public:

protected:
	bool IsAiming;
	bool Fire;
	bool IsDrifting;

	float Damage;
	float Speedmin;
	float reboundloss;
	Vector2 Player_speed;
	float storedvelocity;
	Vector2 m_position;
	Vector2 mouse_position;
	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;
	Sprite* m_pSprite;
	Sprite* m_pBoostPointer;
	Vector2 clickpos;

	bool m_bAlive;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
	float radius;
	float angle;
	float distancebetween;
private:

};

#endif // __Player_H__
