// COMP710 GP Framework
#ifndef SceneSpaceInvadersClone_H
#define SceneSpaceInvadersClone_H
// Local includes:
#include "scene.h"
// Forward declarations:
class Renderer;
class Alieninvader;
class LaserCannon;
class Laserbullet;
// Class declaration:
class SceneSpaceInvadersClone : public Scene
{
	// Member methods:
public:
	SceneSpaceInvadersClone();
	virtual ~SceneSpaceInvadersClone();
	virtual bool Initialise(Renderer& renderer);
	void CheckBulletCollissions();
	bool isAlive();
	void Kill();
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
protected:
private:
	SceneSpaceInvadersClone(const SceneSpaceInvadersClone& SceneSpaceInvadersClone);
	SceneSpaceInvadersClone& operator=(const SceneSpaceInvadersClone& SceneSpaceInvadersClone);
	// Member data:
public:
protected:
	Alieninvader* m_pInvaders[55];
	Laserbullet* m_pLaser[10];
	LaserCannon* m_pCannon;
	int m_iShowCount;
	Renderer* storage;
	float cooldown;
private:
};
#endif // SceneSpaceInvadersClone_H
