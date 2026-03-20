// COMP710 JESSE
#ifndef __SCENEASTEROIDSCLONE_H__
#define __SCENEASTEROIDSCLONE_H__
// Local includes:
#include "scene.h"
// Forward declarations:
class Renderer;
class InputSystem;
class Asteroid;
class Playership;
class Bullet;
// Class declaration:
class Sceneasteroidsclone : public Scene
{
	// Member methods:
public:
	Sceneasteroidsclone();
	virtual ~Sceneasteroidsclone();
	virtual bool Initialise(Renderer& renderer);
	void CreateAsteroid(float angle, float positionX, float positionY, float Size);
	void CheckNeutralCollisions();
	void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
protected:
private:
	Sceneasteroidsclone(const Sceneasteroidsclone& Sceneasteroidsclone);
	Sceneasteroidsclone& operator=(const Sceneasteroidsclone& Sceneasteroidsclone);
	// Member data:
public:
protected:
	Asteroid* m_pAsteroids[100];
	int m_iShowCount;
	Renderer* storage;
	Playership* m_pShip;
	bool Shooting;
	int cooldown;
	Bullet* m_pBullets[10];
	int TotalAsteroids;
	int Score;
private:

};

#endif // __SCENEASTEROIDSCLONE_H__