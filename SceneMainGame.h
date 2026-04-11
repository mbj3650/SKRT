// COMP710 JESSE
#ifndef __SceneMainGame_H__
#define __SceneMainGame_H__
// Local includes:
#include "scene.h"
#include "fmod.hpp"
#include "box2d.h"
// Forward declarations:
class Renderer;
class InputSystem;
class PlayerObject;
class EnemyBase;
class b2WorldDef;
class Player;
class b2WorldId;
class FMOD::System;
class ParticleEmitter;
// Class declaration:
class SceneMainGame : public Scene
{
	// Member methods:
public:
	SceneMainGame();
	virtual ~SceneMainGame();
	virtual bool Initialise(Renderer& renderer);
	void CreateEnemy();
	void SpawnExp(b2Vec2 EnemyPosition, float experiencetodrop);
	void CheckCollisions();
	void EntityColliding(b2ShapeId Shape1, b2ShapeId Shape2);
	void EntityHitting(b2ShapeId Shape1, b2ShapeId Shape2);
	void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
	void SetSystem(FMOD::System& system);
protected:
private:
	SceneMainGame(const SceneMainGame& SceneMainGame);
	SceneMainGame& operator=(const SceneMainGame& SceneMainGame);
	// Member data:
public:
protected:

	static SceneMainGame* sm_MainGameInstance;
	FMOD::System* SoundSystem;
	ParticleEmitter* m_pParticleEmitter;
	EnemyBase* m_pEntityArray[100];
	b2WorldDef* World;
	b2WorldId WorldPointer;
	Renderer* storage;
	PlayerObject* m_pPlayerChar;
	int m_iShowCount;
	bool Shooting;
	int cooldown;
	int TotalEnemies;
	int Score;
	bool helddown;
private:

};

#endif // __SceneMainGame_H__