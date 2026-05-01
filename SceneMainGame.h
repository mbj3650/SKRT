// COMP710 JESSE
#ifndef __SceneMainGame_H__
#define __SceneMainGame_H__
// Local includes:
#include "scene.h"
#include "fmod.hpp"
#include "box2d.h"
#include "UpgradeList.h"
// Forward declarations:
class Renderer;
class InputSystem;
class PlayerObject;
class EnemyBase;
class b2WorldDef;
class Director;
class Player;
class b2WorldId;
class Sprite;
class UpgradeList;
struct UpgradeList::Template;
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
	void SpawnMine(b2Vec2 EnemyPosition);
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
	std::vector <ParticleEmitter*> m_pParticleEmitter;
	std::vector<EnemyBase*>* m_pEntityArray;


	//world variables
	int ScenesubStepCount;
	b2WorldDef* World;
	b2WorldId WorldPointer;


	//renderer variables
	Renderer* storage;
	PlayerObject* m_pPlayerChar;
	Director* m_pDirector;
	int m_iShowCount;



	int gamespeed;
	bool Shooting;
	int cooldown;
	int TotalEntities;
	int Score;
	bool helddown;
	Sprite* m_pCursor;

	float timebeforeunpause; //pause variables
	bool paused;

	UpgradeList UpgradeCopy;
	std::vector<UpgradeList::Template> applicableupgrades;
private:

};

#endif // __SceneMainGame_H__