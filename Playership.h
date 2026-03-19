// COMP710 JESSE
#include "vector2.h"
#ifndef __PLAYERSHIP_H__
#define __PLAYERSHIP_H__

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;
class Vector2;
// Class declaration:
class Playership
{
	// Member methods:
public:
	Playership();
	~Playership();
	Vector2 Position();
	float GetShipAngle();
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);

protected:

private:
	Playership(const Playership& Playership);
	Playership& operator=(const Playership& Playership);

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
	float angle;
private:

};

#endif // __PLAYERSHIP_H__
