// COMP710 JESSE
#include "vector2.h"
#ifndef __Bullet_H__
#define __Bullet_H__

// Forward declarations:
class Renderer;
class Sprite;
// Class declaration:
class Bullet
{
	// Member methods:
public:
	Bullet();
	~Bullet();

	bool Initialise(Vector2 Position, Renderer& renderer, float angle);

	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void Kill();
	bool isAlive();
	float GetRadius();
	Vector2& Position();
	virtual void DebugDraw();
	void ComputeBounds(int width, int height);
protected:

private:
	Bullet(const Bullet& Bullet);
	Bullet& operator=(const Bullet& Bullet);

	// Member data:
public:

protected:
	Vector2 m_position;
	Vector2 velocity;
	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;
	Sprite* m_pSprite;
	bool m_bAlive;
	float speed;
	float radius;
	static int direction;
	static bool Hittingboundary;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
private:

};
#endif // __Bullet_H__