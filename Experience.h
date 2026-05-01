// COMP710 JESSE
#ifndef __Experience_H__
#define __Experience_H__
#include "vector2.h"
#include "EnemyBase.h"
#include "box2d.h"
// Forward declarations:
class Renderer;
class Sprite;
class PlayerObject;
class b2BodyId;
class b2WorldId;
class InputSystem;
class b2ShapeId;
// Class declaration:
class Experience : public EnemyBase
{
	// Member methods:
public:
	Experience();
	~Experience();

	bool Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position, float experiencetodrop) override;
	float getExperienceAngle();
	void Process(float deltaTime) override;
	void ProcessCollision(b2BodyId collidingwith) override;
	void ProcessDamageCollision(b2BodyId collidingwith) override;
	void Draw(Renderer& renderer) override;
	int getSize() override;
	void Kill() override;
	 bool isAlive() override;
	float GetRadius();
	Vector2& Position();
	void ComputeBounds(int width, int height);
protected:

private:
	Experience(const Experience& Experience);
	Experience& operator=(const Experience& Experience);

	// Member data:
public:
	bool isColliding;
	b2BodyId ID;
protected:

	bool Cancollide;
	b2ShapeId shapeId;
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
	float Experiencesize;
	float radius;
	float TimerPostCollide;
	static bool Hittingboundary;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
private:

};

#endif // __Experience_H__


