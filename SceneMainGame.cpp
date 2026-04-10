// COMP710 JESSE
// This include:
#include "SceneMainGame.h"

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "sprite.h"
#include "Player.h"
#include "PlayerObject.h"
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
	: m_pEnemyArray{ 0 }
	, m_iShowCount(0)
	, cooldown(0)//time between shooting
	, Score(0)
{

};
SceneMainGame::~SceneMainGame()
{
	for (int k = 0; k < 100; ++k)
	{
		delete m_pEnemyArray[k];
		m_pEnemyArray[k] = NULL;
	}
	delete m_pPlayerChar;
	m_pPlayerChar = 0;

	SoundSystem->release();
	delete SoundSystem;
	SoundSystem = 0;
	delete storage;
	storage = 0;
	b2DestroyWorld(WorldPointer);
};



bool
SceneMainGame::Initialise(Renderer& renderer)
{
	World = new b2WorldDef();
	*World = b2DefaultWorldDef();
	WorldPointer = b2CreateWorld(World);
	//b2BodyDef wall = b2DefaultBodyDef();//initialize wall
	//b2Vec2 position;//set position of wall
	//position.x = 0;
	//position.y = -10;
	//wall.position = position;
	//b2BodyId wallId = b2CreateBody(WorldPointer, &wall); //make info exist in the world
	//b2Polygon wallBox = b2MakeBox(50.0f, 10.0f); //make a box

	//b2ShapeDef wallShapeDef = b2DefaultShapeDef();//set shape of wall to default
	//b2CreatePolygonShape(wallId, &wallShapeDef, &wallBox);//create it in the world


	storage = &renderer;
	m_pPlayerChar = new PlayerObject();
	m_pPlayerChar->Initialise(renderer, WorldPointer);
	/*m_pParticleEmitter = new ParticleEmitter();
	std::string particle = "..\\assets\\asteroid.png";
	float smoke[3] = { 1 };
	m_pParticleEmitter->Initialise(renderer, particle.c_str(),1,2,1, smoke,0,360,3);*/
	m_iShowCount = 0;
	for (int i = 0; i < 20; i++) {
		CreateEnemy();
		//m_iShowCount++;
	}




	return true;
};

void SceneMainGame::CreateEnemy() {
	for (int z = 0; z < 100; z++) {
		if (m_pEnemyArray[z] == NULL) {
			m_pEnemyArray[z] = new EnemyBase();
			TotalEnemies++;
			m_pEnemyArray[z]->Initialise(*storage, m_pPlayerChar->ID, WorldPointer);
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
			if (B2_ID_EQUALS(m_pPlayerChar->ID, bodyA)) {
				address->ProcessDamageCollision(bodyA);

			}
			else {
				address->ProcessCollision(bodyA);
			}
			
		}
		catch(FMOD_ERRORCALLBACK_INSTANCETYPE) {
			std::cout << "Address wasnt enemy!";
		}
	
}
void
SceneMainGame::Process(float deltatime,InputSystem& inputsystem)
{
	int subStepCount = 4;
	int gamespeed = 2;
	for (int i = 0; i < 100; i++) {
		if (m_pEnemyArray[i] != NULL) {
			if (m_pEnemyArray[i]->isAlive()) {
				delete m_pEnemyArray[i];
			}
			else {
				m_pEnemyArray[i]->Process(deltatime);
			}
			
		}
	}
	m_pPlayerChar->Process(deltatime, inputsystem);//check fo rcollissions
	int result = inputsystem.GetMouseButtonState(SDL_BUTTON_LEFT);
	/*m_pParticleEmitter->SetParticlePosition(m_pPlayerChar->Position());
	m_pParticleEmitter->Process(deltatime);*/
	b2World_Step(WorldPointer, deltatime*gamespeed, subStepCount);


	// In game loop after stepping physics
	b2ContactEvents contactEvents = b2World_GetContactEvents(WorldPointer);
	for (int i = 0; i < contactEvents.endCount; ++i)
	{
		b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;
		EntityColliding(endEvent->shapeIdA, endEvent->shapeIdB);
	}


	/*if (m_pPlayerChar->CanDamage()) {
		for (int i = 0; i < 100; i++) {
			if (m_pEnemyArray[i] != NULL) {
				if (m_pEnemyArray[i]->isColliding == false) {
					double distance =
						sqrt(
							pow(((m_pEnemyArray[i]->Position().x) - (m_pPlayerChar->Position().x)), 2)
							+ pow(((m_pEnemyArray[i]->Position().y) - (m_pPlayerChar->Position().y)), 2)
						) - m_pPlayerChar->GetRadius() - m_pEnemyArray[i]->GetRadius();
					if (distance <= 0) {
						m_pEnemyArray[i]->ProcessCollision(m_pPlayerChar->ID, deltatime);
						break;
					}
				}
			}
		}
	}
	


	for (int i = 0; i < 100; i++) {
		if (m_pEnemyArray[i] != NULL) {
			if (m_pEnemyArray[i]->isColliding == false) {
				for (int ib = 0; ib < 100; ib++) {
					if (m_pEnemyArray[ib] != NULL && ib != i) {
						if (m_pEnemyArray[ib]->isColliding == false) {
							double distance =
								sqrt(
									pow(((m_pEnemyArray[i]->Position().x) - (m_pEnemyArray[ib]->Position().x)), 2)
									+ pow(((m_pEnemyArray[i]->Position().y) - (m_pEnemyArray[ib]->Position().y)), 2)
								) - m_pEnemyArray[ib]->GetRadius() - m_pEnemyArray[i]->GetRadius();
							if (distance <= 0) {
								m_pEnemyArray[i]->ProcessCollision(m_pEnemyArray[ib], deltatime);
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
	for (int i = 0; i < 100; i++) {
		if (m_pEnemyArray[i] != NULL) {
			m_pEnemyArray[i]->Draw(renderer);
		}
	}

	//m_pParticleEmitter->Draw(renderer);
	m_pPlayerChar->Draw(renderer);
};



void SceneMainGame::DebugDraw
()
{
	ImGui::Text("Scene: SKRT_MAIN");
	ImGui::Text("Score: %d", Score);
	ImGui::Text("Total Enemies %d", TotalEnemies);
	m_pPlayerChar->DebugDraw();
	//m_pParticleEmitter->DebugDraw();
}

void SceneMainGame::SetSystem(FMOD::System& system) {
	*SoundSystem = system;
}
