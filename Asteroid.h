// COMP710 JESSE
#ifndef __ASTEROID_H__
#define __ASTEROID_H__
#include "vector2.h"
// Forward declarations:
class Renderer;
class Sprite;
// Class declaration:
class Asteroid
{
	// Member methods:
public:
	Asteroid();
	~Asteroid();

	bool Initialise(Renderer& renderer, float angle,int size);
	float getAsteroidAngle();
	void Process(float deltaTime);
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
	Asteroid(const Asteroid& Asteroid);
	Asteroid& operator=(const Asteroid& Asteroid);

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
	int asteroidsize;
	float radius;
	static int direction;
	static bool Hittingboundary;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
private:

};

#endif // __ASTEROID_H__


