// COMP710 JESSE
// This include:
#include "Mine.h"

// Local includes:
#include "PlayerObject.h"
#include <conio.h>
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
#include "inlinehelpers.h"
#include "lib/imgui/imgui.h"
#include <box2d.h>
#include <stdexcept>
#include <iostream>
Mine::Mine()
{

};
Mine::~Mine()
{
	m_pSprite = 0;
};

bool
Mine::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position)
{
	type = 70;//type
	m_pPlayer = playerAddress;//player address
	m_pSprite = renderer.CreateSprite("..\\assets\\enemies\\mine.png");//sprite
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;//basic enemy stuff
	flickertimer = 2;//blink red every so often
	health = 1;
	damage = 5;//damage to player
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_pSprite->SetScale(0.06f * 307/m_pSprite->GetWidth());//set size

		//CREATE BODY FOR THE WORLD TO USE AS SHAPE REFERENCE
	b2BodyDef WorldObj = b2DefaultBodyDef();
	if (position.x == -9999) {
		int sidetospawn = rand() % 4;//pick a random side to spawn from
		switch (sidetospawn) {
		case 1:
			WorldObj.position.x = 0 - m_pSprite->GetWidth();
			WorldObj.position.y = static_cast<float>(GetRandom(0, SCREEN_HEIGHT));
			//spawn on left anywhere
			break;
		case 2:
			WorldObj.position.x = static_cast<float>(SCREEN_WIDTH) + m_pSprite->GetWidth();
			WorldObj.position.y = static_cast<float>(GetRandom(0, SCREEN_HEIGHT));
			//spawn on right anywhere
			break;
		case 3:
			WorldObj.position.x = static_cast<float>(GetRandom(0, SCREEN_WIDTH));
			WorldObj.position.y = static_cast<float>(SCREEN_HEIGHT) + m_pSprite->GetHeight();
			//spawn on bottom anywhere
			break;
		default:
			WorldObj.position.x = static_cast<float>(GetRandom(0, SCREEN_WIDTH));
			WorldObj.position.y = 0 - m_pSprite->GetHeight();
			//spawn top anywhere
			break;
		}
	}
	else {
		WorldObj.position.x = position.x;
		WorldObj.position.y = position.y;
	}
	
	ID = b2CreateBody(WorldID, &WorldObj);
	b2Body_SetType(ID, b2_dynamicBody);
	b2Body_SetUserData(ID, this);
	b2Polygon box = b2MakeRoundedBox(4, 4, 5.0f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1000.0f;
	shapeDef.friction = 0.1f;


	shapeDef.filter.categoryBits = 0x0023;//i am
	shapeDef.filter.maskBits = 0x0001;//i collide with the player ONLY

	shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);

	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	b2Body_SetAwake(ID, true);
	b2Shape_EnableContactEvents(shapeId, true);
	b2Vec2 speedVec = { 0,0 };
	b2Body_SetLinearVelocity(ID, speedVec);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
};

void
Mine::Process(float deltaTime)
{
	if (m_bAlive) {
		if (flickertimer > 0) {//goes from 2 to 0
			flickertimer -= deltaTime;
		}
		else {
			flickertimer = 2;
		}
		if (lifetime > 0) {//if still alive
			lifetime -= deltaTime;//tick down timer
		}
		else {
			m_bAlive = false;
		}
		m_pSprite->SetAlpha(lifetime / 5);//slowly fadeout
		m_pSprite->SetRedTint(1.0f);
		m_pSprite->SetGreenTint(2 - flickertimer);//will start at 0 meaning full red into 1 meaning white
		m_pSprite->SetBlueTint(2 - flickertimer);
		m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
		m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
		m_pSprite->Process(deltaTime);
	}
};

void Mine::ProcessDamageCollision(b2BodyId collidingwith) {//always damage player, dont take damage
	try {//attempt player damage
		PlayerObject* address = reinterpret_cast<PlayerObject*>(b2Body_GetUserData(collidingwith));
		address->takedamage(damage);
		m_bAlive = false;//explode
	}
	catch (...) {
	}
}

void
Mine::Draw(Renderer& renderer)//draw it
{
	if (m_bAlive) {
		m_pSprite->Draw(renderer);
	}
	
};