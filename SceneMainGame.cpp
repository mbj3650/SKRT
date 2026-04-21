// COMP710 JESSE
// This include:
#include "SceneMainGame.h"

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "sprite.h"
#include "UpgradeList.h"
#include "PlayerObject.h"
#include "Experience.h"
#include "EnemyBase.h"
// Library includes:
#include <cassert>
#include "lib/imgui/imgui.h"
#include "inlinehelpers.h"
#include <string>
#include <fmod.hpp>
#include <iostream>
#include "box2d.h"
#include "particleemitter.h"
SceneMainGame* SceneMainGame::sm_MainGameInstance = 0;
SceneMainGame::SceneMainGame()
	: m_pEntityArray{ 0 }
	, m_iShowCount(0)
	, cooldown(0)//time between shooting
	, Score(0)
	, timebeforeunpause(0)
{

};
SceneMainGame::~SceneMainGame()
{
	for (int k = 0; k < 100; ++k)
	{
		if (b2Body_IsValid(m_pEntityArray[k]->ID)) {
			b2DestroyBody(m_pEntityArray[k]->ID);
		}
		delete m_pEntityArray[k];
		m_pEntityArray[k] = NULL;
	}
	delete[] m_pEntityArray;
	delete m_pPlayerChar;
	m_pPlayerChar = 0;

	SoundSystem->release();
	delete SoundSystem;
	SoundSystem = 0;
	delete storage;
	storage = 0;

	b2DestroyWorld(WorldPointer);
	delete World;
	World = 0;
};



bool
SceneMainGame::Initialise(Renderer& renderer)
{

	World = new b2WorldDef();
	*World = b2DefaultWorldDef();
	WorldPointer = b2CreateWorld(World);
	b2World_SetGravity(WorldPointer,{ 0,0 });
	ScenesubStepCount = 16;
	gamespeed = 2;
	storage = &renderer;
	m_pPlayerChar = &m_pPlayerChar->GetInstance();
	m_pPlayerChar->Initialise(renderer, WorldPointer);
	int SCREEN_WIDTH = storage->GetWidth();
	int SCREEN_HEIGHT = storage->GetHeight();

	//PARTICLE SET UP

	float defaultcolor[3] = { 1,1,1 };//dont change color of particle

	ParticleEmitter* smokeparticles = new ParticleEmitter();
	std::string particle = "..\\assets\\drift.png";

	smokeparticles->Initialise(renderer, particle.c_str(),0.1,0.3,600, defaultcolor,0,360,3);

	m_pParticleEmitter[0] = smokeparticles;



	ParticleEmitter* deathparticles = new ParticleEmitter();
	particle = "..\\assets\\deathexplosion.png";
	deathparticles->Initialise(renderer, particle.c_str(), -1, 0.3, 0, defaultcolor, 0, 360, 1);

	m_pParticleEmitter[1] = deathparticles;


	m_iShowCount = 0;
	for (int i = 0; i < 20; i++) {
		CreateEnemy();
		//m_iShowCount++;
	}
	UpgradeCopy.Initialize(renderer, m_pPlayerChar);
	return true;
};



void
SceneMainGame::Process(float deltatime,InputSystem& inputsystem)
{
	if (timebeforeunpause >= 0) {
		timebeforeunpause -= 2 * deltatime;
	}
	if (inputsystem.GetKeyState((SDL_SCANCODE_P)) == BS_HELD) {
		if (timebeforeunpause <= 0) {
			timebeforeunpause = 2;
			if (paused == true) {
				paused = false;
				UpgradeCopy.pausemenuupgrades = false;
			}
			else {
				UpgradeCopy.pausemenuupgrades = true;
				paused = true;
			}
		}

	}
	if (m_pPlayerChar->PlayerNeedsUpgrade == true) {
		if (UpgradeCopy.showingupgrades == false) {
			UpgradeCopy.PickThree();
		}
		UpgradeCopy.Process(deltatime, inputsystem);
		deltatime = 0;
	}

	else if (paused == true) {//if paused or a menu is up, stop everything
		deltatime = 0;
	}

	m_pPlayerChar->Process(deltatime, inputsystem);
	if (m_pPlayerChar->CanDamage()){
		m_pParticleEmitter[0]->turnon();//enable for speed particles
		m_pParticleEmitter[0]->SetParticlePosition(m_pPlayerChar->Position());//setpositions
	}
	else{
		m_pParticleEmitter[0]->turnoff();
	}
	for (int i = 0; i < 20; i++) {
		if (m_pParticleEmitter[i] != NULL) {
			m_pParticleEmitter[i]->Process(deltatime);
		}
	}
	
	b2World_Step(WorldPointer, deltatime*gamespeed, ScenesubStepCount);
	

	// In game loop after stepping physics
	b2ContactEvents contactEvents = b2World_GetContactEvents(WorldPointer);
	for (int i = 0; i < contactEvents.beginCount; ++i)
	{
		b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
		EntityColliding(beginEvent->shapeIdA, beginEvent->shapeIdB);
	}


	// In game loop after stepping physics
	for (int i = 0; i < contactEvents.hitCount; ++i)
	{
		b2ContactHitEvent* beginEvent = contactEvents.hitEvents + i;
		EntityHitting(beginEvent->shapeIdA, beginEvent->shapeIdB);
	}


	for (int i = 0; i < 100; i++) {
		if (m_pEntityArray[i] != NULL) {
			m_pEntityArray[i]->Process(deltatime);
			if (!m_pEntityArray[i]->isAlive()) {//and the body is valid
				if (b2Body_IsValid(m_pEntityArray[i]->ID)) {
					m_pParticleEmitter[1]->SetParticlePosition(b2Body_GetPosition(m_pEntityArray[i]->ID));
					m_pParticleEmitter[1]->Spawn();
					b2DestroyBody(m_pEntityArray[i]->ID);
				}
					delete m_pEntityArray[i];
					m_pEntityArray[i] = 0;
			}
		}
	}

	/*if (m_pPlayerChar->CanDamage()) {
		for (int i = 0; i < 100; i++) {
			if (m_pEntityArray[i] != NULL) {
				if (m_pEntityArray[i]->isColliding == false) {
					double distance =
						sqrt(
							pow(((m_pEntityArray[i]->Position().x) - (m_pPlayerChar->Position().x)), 2)
							+ pow(((m_pEntityArray[i]->Position().y) - (m_pPlayerChar->Position().y)), 2)
						) - m_pPlayerChar->GetRadius() - m_pEntityArray[i]->GetRadius();
					if (distance <= 0) {
						m_pEntityArray[i]->ProcessCollision(m_pPlayerChar->ID, deltatime);
						break;
					}
				}
			}
		}
	}
	


	for (int i = 0; i < 100; i++) {
		if (m_pEntityArray[i] != NULL) {
			if (m_pEntityArray[i]->isColliding == false) {
				for (int ib = 0; ib < 100; ib++) {
					if (m_pEntityArray[ib] != NULL && ib != i) {
						if (m_pEntityArray[ib]->isColliding == false) {
							double distance =
								sqrt(
									pow(((m_pEntityArray[i]->Position().x) - (m_pEntityArray[ib]->Position().x)), 2)
									+ pow(((m_pEntityArray[i]->Position().y) - (m_pEntityArray[ib]->Position().y)), 2)
								) - m_pEntityArray[ib]->GetRadius() - m_pEntityArray[i]->GetRadius();
							if (distance <= 0) {
								m_pEntityArray[i]->ProcessCollision(m_pEntityArray[ib], deltatime);
								break;
							}
						}
					}
				}
			}
		}
	}
		}*/



};

void
SceneMainGame::CheckCollisions() {

}

void
SceneMainGame::Draw(Renderer& renderer)
{

	for (int i = 0; i < 20; i++) {
		if (m_pParticleEmitter[i] != NULL) {
			m_pParticleEmitter[i]->Draw(renderer);
		}
	}
	for (int i = 0; i < 100; i++) {
		if (m_pEntityArray[i] != NULL) {
			if (m_pEntityArray[i]->isAlive()) {
				m_pEntityArray[i]->Draw(renderer);
			}
			
		}
	}

	m_pPlayerChar->Draw(renderer);
	UpgradeCopy.Draw(renderer);
	if (m_pPlayerChar->PlayerNeedsUpgrade) {
		
	}
};



void SceneMainGame::DebugDraw
()
{
	ImGui::Text("Scene: SKRT_MAIN");
	ImGui::Text("Score: %d", Score);
	ImGui::Text("Total Enemies %d", TotalEnemies);
	m_pPlayerChar->DebugDraw();
	UpgradeCopy.DebugDraw();
	//m_pParticleEmitter->DebugDraw();
}

void SceneMainGame::SetSystem(FMOD::System& system) {
	*SoundSystem = system;
}

void SceneMainGame::CreateEnemy() {
	for (int z = 0; z < 100; z++) {
		if (m_pEntityArray[z] == NULL) {
			m_pEntityArray[z] = new EnemyBase();
			TotalEnemies++;
			m_pEntityArray[z]->Initialise(*storage, m_pPlayerChar->ID, WorldPointer);
			break;
		}
	}
}

void SceneMainGame::SpawnExp(b2Vec2 EnemyPosition, float experiencetodrop) {
	for (int z = 0; z < 100; z++) {
		if (m_pEntityArray[z] == NULL) {
			m_pEntityArray[z] = new Experience();
			m_pEntityArray[z]->Initialise(*storage, m_pPlayerChar->ID, WorldPointer, EnemyPosition, experiencetodrop);
			break;
		}
	}
}

void
SceneMainGame::EntityColliding(b2ShapeId Shape1, b2ShapeId Shape2) {

	b2BodyId bodyA = b2Shape_GetBody(Shape1);
	b2BodyId bodyB = b2Shape_GetBody(Shape2);
	try {
		EnemyBase* address = reinterpret_cast<EnemyBase*>(b2Body_GetUserData(bodyB));
		int getType = address->type;
		int isplayer = 0;
		//if body a is player
		if ((B2_ID_EQUALS(m_pPlayerChar->ID, bodyA))) {
			isplayer = 1;
		}
		if ((B2_ID_EQUALS(m_pPlayerChar->ID, bodyB))) {
			isplayer = 2;
		}
		if (isplayer != 0) {
			switch (getType) {
			case 50:
				if (isplayer == 1) {
					address->ProcessDamageCollision(bodyA);
				}
				else {
					address->ProcessDamageCollision(bodyB);
				}

				break;
			case 100:
				if (isplayer == 1) {
					address->ProcessDamageCollision(bodyA);
				}
				else {
					address->ProcessDamageCollision(bodyB);
				}
				if (!address->isAlive()) {
					b2Vec2 Position = b2Body_GetPosition(bodyB);
					SpawnExp(Position, address->experiencetodrop);
				}
				break;
			}
		}



		else {
			//else process entity to entity collission
			address->ProcessCollision(bodyA);
		}

	}
	catch (...) {
		std::cout << "Address wasnt enemy!";
	}

}


void
SceneMainGame::EntityHitting(b2ShapeId Shape1, b2ShapeId Shape2) {

	b2BodyId bodyA = b2Shape_GetBody(Shape1);
	b2BodyId bodyB = b2Shape_GetBody(Shape2);
	try {
		EnemyBase* address = reinterpret_cast<EnemyBase*>(b2Body_GetUserData(bodyB));
		int getType = address->type;
		int isplayer = 0;
		//if body a is player
		if ((B2_ID_EQUALS(m_pPlayerChar->ID, bodyA))) {
			isplayer = 1;
		}
		if ((B2_ID_EQUALS(m_pPlayerChar->ID, bodyB))) {
			isplayer = 2;
		}
		if (isplayer != 0) {
			switch (getType) {
			case 50:
				if (isplayer == 1) {
					address->ProcessDamageCollision(bodyA);
				}
				else {
					address->ProcessDamageCollision(bodyB);
				}

				break;
			}
		}



		else {
			//else process entity to entity collission
			address->ProcessCollision(bodyA);
		}

	}
	catch (FMOD_ERRORCALLBACK_INSTANCETYPE) {
		std::cout << "Address wasnt enemy!";
	}

}
