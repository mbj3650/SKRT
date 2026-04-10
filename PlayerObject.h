// COMP710 JESSE
#ifndef __PlayerObject_H__
#define __PlayerObject_H__
#include "vector2.h"
#include "box2d.h"
// Forward declarations:
class Renderer;
class Sprite;
class Player;
class b2Vec2;
class Vector2;
class b2BodyId;
class b2WorldId;
class InputSystem;
// Class declaration:
class PlayerObject
{
	// Member methods:
public:
	PlayerObject();
	~PlayerObject();

	bool Initialise(Renderer& renderer, Player* playerAddress);
	void SetPosition(b2Vec2 position);
	bool Initialise(Renderer& renderer, b2WorldId WorldId);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);
	void Kill();
	float getSpeed();
	float GetRadius();
	b2Vec2 Position();
	bool CanDamage();
	float getDamage();
	float GetShipAngle();
	void DebugDraw();
	void ComputeBounds(int width, int height);
protected:

private:
	PlayerObject(const PlayerObject& PlayerObject);
	PlayerObject& operator=(const PlayerObject& PlayerObject);

	// Member data:
public:
	bool colliding;
	bool isColliding;
	b2BodyId ID;
protected:
	float ratio;
	float speedboost;
	int maxdistance;
	bool IsAiming;
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

#endif // __PlayerObject_H__


