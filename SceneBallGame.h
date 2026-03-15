// COMP710 GP Framework
#ifndef SCENEBALLGAME_H
#define SCENEBALLGAME_H
// Local includes:
#include "scene.h"
// Forward declarations:
class Renderer;
class Ball;
class EntityBall;
// Class declaration:
class SceneBallGame : public Scene
{
	// Member methods:
public:
	SceneBallGame();
	virtual ~SceneBallGame();
	virtual bool Initialise(Renderer& renderer);
	void CheckNeutralCollisions();
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	void makegoodball(int ballstomake, Renderer& renderer);
	void makebadball(int ballstomake, Renderer& renderer);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
protected:
private:
	SceneBallGame(const SceneBallGame& SceneBallGame);
	SceneBallGame& operator=(const SceneBallGame& SceneBallGame);
	// Member data:
public:
protected:
	EntityBall* m_pBalls[100];
	int Goodball;
	int Badball;
	int m_iShowCount;
	Renderer* storage;
private:
};
#endif // SCENEBALLGAME_H
