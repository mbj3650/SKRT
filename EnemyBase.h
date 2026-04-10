// COMP710 JESSE
#ifndef __EnemyBase_H__
#define __EnemyBase_H__
#include "vector2.h"
#include "box2d.h"
// Forward declarations:
class Renderer;
class Sprite;
class PlayerObject;
class b2BodyId;
class b2WorldId;
class InputSystem;
// Class declaration:
class EnemyBase
{
	// Member methods:
public:
	EnemyBase();
	~EnemyBase();

	bool Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID);

	void startContact();
	void endContact();


	float getEnemyBaseAngle();
	void Process(float deltaTime);
	void ProcessDamageCollision(b2BodyId collidingwith);
	void ProcessCollision(b2BodyId collidingwith);
	void Draw(Renderer& renderer);
	void Kill();
	bool isAlive();
	float GetRadius();
	Vector2& Position();
	int getSize();
	void RandomiseSize();
	virtual void DebugDraw();
	void ComputeBounds(int width, int height);
protected:

private:
	EnemyBase(const EnemyBase& EnemyBase);
	EnemyBase& operator=(const EnemyBase& EnemyBase);

	// Member data:
public:
	bool isColliding;
	b2BodyId ID;
protected:
	float health;
	b2BodyId m_pPlayer;
	Sprite* m_pSprite;
	Vector2 m_position;
	Vector2 velocity;
	Vector2 offsetvelocity;
	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;
	bool m_bAlive;
	float angle;
	float speed;
	int EnemyBasesize;
	float radius;
	float TimerPostCollide;
	static bool Hittingboundary;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
private:

};

#endif // __EnemyBase_H__


