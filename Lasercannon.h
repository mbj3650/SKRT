// COMP710 JESSE
#include "vector2.h"
#ifndef __LASERCANNON_H__
#define __LASERCANNON_H__

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
// Class declaration:
class LaserCannon
{
	// Member methods:
public:
	LaserCannon();
	~LaserCannon();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);
	bool isAlive();
	void Kill();
	float GetRadius();

	Vector2& Position();
	virtual void DebugDraw();
protected:
	void ComputeBounds(int width, int height);
private:
	LaserCannon(const LaserCannon& LaserCannon);
	LaserCannon& operator=(const LaserCannon& LaserCannon);

	// Member data:
public:

protected:
	Vector2 m_position;
	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;
	Sprite* m_pSprite;
	bool m_bAlive;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
	float radius;
private:

};

#endif // __LASERCANNON_H__
