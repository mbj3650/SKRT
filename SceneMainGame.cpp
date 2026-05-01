// COMP710 JESSE
// This include:
#include "SceneMainGame.h"

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "sprite.h"
#include "Director.h"
#include "UpgradeList.h"
#include "PlayerObject.h"
#include "Experience.h"
#include "EnemyBase.h"
#include "Minelayer.h"
#include "Mine.h"
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
	: m_iShowCount(0)
	, m_pEntityArray{ 0 }
	, cooldown(0)//time between shooting
	, Score(0)
	, timebeforeunpause(0)
{

};
SceneMainGame::~SceneMainGame()
{
	
	for (int k = 0; k < m_pEntityArray->size(); ++k)
	{
		if (b2Body_IsValid(m_pEntityArray->at(k)->ID)) {
			b2DestroyBody(m_pEntityArray->at(k)->ID);
		}
	}
	m_pEntityArray->clear();
	delete m_pEntityArray;
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
	srand(time(NULL));
	m_pEntityArray = new std::vector<EnemyBase*>;
	std::cout << "cap"<<m_pEntityArray->capacity() << "\n";
	m_pDirector = new Director();
	World = new b2WorldDef();
	*World = b2DefaultWorldDef();
	WorldPointer = b2CreateWorld(World);
	b2World_SetGravity(WorldPointer,{ 0,0 });
	ScenesubStepCount = 16;
	gamespeed = 2;
	storage = &renderer;
	m_pPlayerChar = &m_pPlayerChar->GetInstance();
	m_pPlayerChar->Initialise(renderer, WorldPointer);
	int SCREEN_WIDTH = renderer.GetWidth();
	int SCREEN_HEIGHT = renderer.GetHeight();

	//PARTICLE SET UP

	float defaultcolor[3] = { 1,1,1 };//dont change color of particle

	ParticleEmitter* smokeparticles = new ParticleEmitter();
	std::string particle = "..\\assets\\drift.png";

	smokeparticles->Initialise(renderer, particle.c_str(),0.1,0.3,600, defaultcolor,0,360,3);

	m_pParticleEmitter.push_back(smokeparticles);

	m_pCursor = renderer.CreateSprite("..\\assets\\crosshair.png");


	ParticleEmitter* deathparticles = new ParticleEmitter();
	particle = "..\\assets\\deathexplosion.png";
	deathparticles->Initialise(renderer, particle.c_str(), -1, 0.3, 0, defaultcolor, 0, 360, 1);

	m_pParticleEmitter.push_back(deathparticles);


	ParticleEmitter* mineparticles = new ParticleEmitter();
	particle = "..\\assets\\mineexplosion.png";
	mineparticles->Initialise(renderer, particle.c_str(), -1, 0.3, 0, defaultcolor, 0, 360, 1);

	m_pParticleEmitter.push_back(mineparticles);

	

	UpgradeCopy.Initialize(renderer, m_pPlayerChar);
	m_pDirector->Initialise(renderer, *m_pEntityArray, m_pPlayerChar, WorldPointer);

	return true;
};



void
SceneMainGame::Process(float deltatime,InputSystem& inputsystem)
{
	if (!m_pPlayerChar->Aiming()) {
		m_pCursor->SetX(inputsystem.GetMousePosition().x);
		m_pCursor->SetY(inputsystem.GetMousePosition().y);
	}
	else {
		m_pCursor->SetX(m_pPlayerChar->clickpos.x);
		m_pCursor->SetY(m_pPlayerChar->clickpos.y);
	}

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

	m_pDirector->Process(deltatime);
	m_pPlayerChar->Process(deltatime, inputsystem);
	if (m_pPlayerChar->CanDamage()){
		m_pParticleEmitter.at(0)->turnon();//enable for speed particles
		m_pParticleEmitter.at(0)->SetParticlePosition(m_pPlayerChar->Position());//setpositions
	}
	else{
		m_pParticleEmitter.at(0)->turnoff();
	}
	for (int i = 0; i < m_pParticleEmitter.size(); i++) {
		if (m_pParticleEmitter.at(i) != NULL) {
			m_pParticleEmitter.at(i)->Process(deltatime);
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


	for (int i = 0; i < m_pEntityArray->size(); i++) {
			m_pEntityArray->at(i)->Process(deltatime);
			if (m_pEntityArray->at(i)->type == 102) {
				
				try {//attempt minespawn
					Minelayer* MineEntity = reinterpret_cast<Minelayer*>(m_pEntityArray->at(i));
					if (MineEntity->needsmine == true) {//if needs mine
						SpawnMine(b2Body_GetPosition(m_pEntityArray->at(i)->ID));
						MineEntity->MinePlaced();//turn off mine call
					}
				}
				catch (...) {
				}
			}
			if (!m_pEntityArray->at(i)->isAlive()) {//and the body is valid
				if (b2Body_IsValid(m_pEntityArray->at(i)->ID)) {
					if (m_pEntityArray->at(i)->type == 70) {
						m_pParticleEmitter.at(2)->SetParticlePosition(b2Body_GetPosition(m_pEntityArray->at(i)->ID));
						m_pParticleEmitter.at(2)->Spawn();
					}
					else {
						m_pParticleEmitter.at(1)->SetParticlePosition(b2Body_GetPosition(m_pEntityArray->at(i)->ID));
						m_pParticleEmitter.at(1)->Spawn();
					}
					b2DestroyBody(m_pEntityArray->at(i)->ID);
				}
					m_pEntityArray->erase(m_pEntityArray->begin()+i);
			}
	}
};

void
SceneMainGame::CheckCollisions() {

}

void
SceneMainGame::Draw(Renderer& renderer)
{

	for (int i = 0; i < m_pParticleEmitter.size(); i++) {
		if (m_pParticleEmitter.at(i) != NULL) {
			m_pParticleEmitter.at(i)->Draw(renderer);
		}
	}
	for (int i = 0; i < m_pEntityArray->size(); i++) {
		m_pEntityArray->at(i)->Draw(renderer);
	}

	m_pPlayerChar->Draw(renderer);
	UpgradeCopy.Draw(renderer);
	m_pCursor->Draw(renderer);
};



void SceneMainGame::DebugDraw
()
{
	ImGui::Text("Scene: SKRT_MAIN");
	ImGui::Text("Score: %d", Score);

	//ImGui::Text("Total Entities %d", m_pEntityArray->size());
	m_pPlayerChar->DebugDraw();
	UpgradeCopy.DebugDraw();
	m_pDirector->DebugDraw();
	//m_pParticleEmitter->DebugDraw();
}

void SceneMainGame::SetSystem(FMOD::System& system) {
	*SoundSystem = system;
}

void SceneMainGame::SpawnExp(b2Vec2 EnemyPosition, float experiencetodrop) {
	m_pEntityArray->push_back(new Experience());
	m_pDirector->AddCredits(experiencetodrop);
	m_pEntityArray->at(m_pEntityArray->size()-1)->Initialise(*storage, m_pPlayerChar->ID, WorldPointer, EnemyPosition, experiencetodrop);
}


void SceneMainGame::SpawnMine(b2Vec2 EnemyPosition) {
	m_pEntityArray->push_back(new Mine());
	m_pEntityArray->at(m_pEntityArray->size() - 1)->Initialise(*storage, m_pPlayerChar->ID, WorldPointer, EnemyPosition);
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
			case 70:
				if (isplayer == 1) {
					address->ProcessDamageCollision(bodyA);
				}
				else {
					address->ProcessDamageCollision(bodyB);
				}

				break;
			case 100:
			case 102:
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
