// COMP710 JESSE
#include "vector2.h"
#ifndef __LASERBULLET_H__
#define __LASERBULLET_H__

// Forward declarations:
class Renderer;
class Sprite;
// Class declaration:
class Laserbullet
{
	// Member methods:
public:
	Laserbullet();
	~Laserbullet();

	bool Initialise(Vector2 Position, Renderer& renderer);
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
	Laserbullet(const Laserbullet& Laserbullet);
	Laserbullet& operator=(const Laserbullet& Laserbullet);

	// Member data:
public:

protected:
	Vector2 m_position;
	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;
	Sprite* m_pSprite;
	bool m_bAlive;
	float radius;
	static int direction;
	static bool Hittingboundary;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
private:

};
#endif // __LASERBULLET_H__