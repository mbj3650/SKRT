// COMP710 JESSE
#ifndef __TREE_H__
#define __TREE_H__
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
class Tree : public EnemyBase
{
	// Member methods:
public:
	Tree();
	~Tree() override;

	bool Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID) override;
	void Process(float deltaTime) override;
	void ProcessCollision(b2BodyId collidingwith) override;
	void ProcessDamageCollision(b2BodyId collidingwith) override;
	void Draw(Renderer& renderer) override;
	void Kill() override;
	float GetRadius();
	bool isAlive() override;
	Vector2& Position();
	void ComputeBounds(int width, int height);
protected:

private:
	Tree(const Tree& Tree);
	Tree& operator=(const Tree& Tree);

	// Member data:
public:
	bool isColliding;
	b2BodyId ID;
protected:
	float health;
	float damage;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	//basic variables for how it should interact with the world + sprite info
	b2ShapeId shapeId;
	b2BodyId m_pPlayer;
	Sprite* m_pSprite;
	Vector2 m_position;
	Vector2 velocity;
	Vector2 offsetvelocity;
	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;
	bool m_bAlive;

	//movement and interaction variables
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

#endif // __TREE_H__


