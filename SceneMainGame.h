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
class AnimatedSprite;
class PlayerObject;
class EnemyBase;
class b2WorldDef;
class Director;
class Player;
class Hud;
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
	void Restart();
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();
	void SetSystem(FMOD::System* system);
protected:
private:
	SceneMainGame(const SceneMainGame& SceneMainGame);
	SceneMainGame& operator=(const SceneMainGame& SceneMainGame);
	// Member data:
public:
protected:

	FMOD::System* SoundSystem;
	std::vector <ParticleEmitter*> m_pParticleEmitter;
	std::vector<EnemyBase*>* m_pEntityArray;
	std::vector<FMOD::Sound*> soundlist;

	//world variables
	int ScenesubStepCount;
	b2WorldDef* World;
	b2WorldId WorldPointer;


	//renderer variables
	Renderer* storage;
	PlayerObject* m_pPlayerChar;
	Director* m_pDirector;
	int m_iShowCount;
	int selectedentity;
	Hud* UserInfo;

	int gamespeed;
	bool Shooting;
	bool hasclicked;
	int cooldown;
	int TotalEntities;
	int Score;
	bool helddown;
	Sprite* m_pCursor;
	Sprite* m_pPause;
	Sprite* m_pTutorial;
	float timebeforeunpause; //pause variables
	bool paused;
	enum SoundTypes {
		KILL,
		LEVELUP,
		EXPLOSION,
		MINELAYING,
		SKIP,
		SELECT,
		HIT,
		RADIATION,
	};
	
	UpgradeList* UpgradeCopy;
	std::vector<UpgradeList::Template> applicableupgrades;
private:

};

#endif // __SceneMainGame_H__