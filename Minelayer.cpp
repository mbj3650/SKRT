// COMP710 JESSE
// This include:
#include "Minelayer.h"

// Local includes:
#include "renderer.h"

// Library includes:
#include <cassert>
#include "sprite.h"
#include "AnimatedSprite.h"
#include "inlinehelpers.h"
#include "PlayerObject.h"
Minelayer::Minelayer()
{

};
Minelayer::~Minelayer()
{

};

bool
Minelayer::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position)
{
	type = 100;
	experiencetodrop = GetRandom(1, 5);
	m_pPlayer = playerAddress;
	m_pSprite = renderer.CreateAnimatedSprite("..\\assets\\enemies\\minelayer.png");
	m_pSprite->SetLooping(true);
	m_pSprite->Animate();
	m_pSprite->SetupFrames(307, 307);
	m_pSprite->SetFrameDuration(0.2);
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	SCREEN_WIDTH = renderer.GetWidth();
	SCREEN_HEIGHT = renderer.GetHeight();
	target.x = GetRandom(100, SCREEN_WIDTH - 100);
	target.y = GetRandom(100, SCREEN_HEIGHT - 100);
	travelling = true;//immeadiately begin looking for a spot to travel to
	m_bAlive = true;
	health = 60;
	speed = GetRandom(50, 80);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_pSprite->SetScale(0.1f * 307/m_pSprite->GetWidth());
	
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
	b2Polygon box = b2MakeRoundedBox(6, 6, 5.0f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.1f;


	shapeDef.filter.categoryBits = 0x0022;//i am
	shapeDef.filter.maskBits = 0x0001 | 0x0002;//i collide with enemies and player

	shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);

	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	b2Body_SetAwake(ID, true);
	return true;
	b2Shape_EnableContactEvents(shapeId, true);
	b2Vec2 speedVec = { speed * (cos(m_pSprite->GetAngle())) , speed * (sin(m_pSprite->GetAngle())) };
	b2Body_SetLinearVelocity(ID, speedVec);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

void
Minelayer::Process(float deltaTime)
{
	double distance =
		sqrt(
			pow(((b2Body_GetPosition(ID).x) - (target.x)), 2)
			+ pow(((b2Body_GetPosition(ID).y) - (target.y)), 2)
		) - m_pSprite->GetWidth()/2;//get distance to player


	if (distance < 10) {
		if (travelling == true) {//set travelling to false
			travelling = false;
			timetillnextspot = 3;//also set cooldown
		}
				
		b2Vec2 velocityVec = { b2Body_GetLinearVelocity(ID).x / 1.1,b2Body_GetLinearVelocity(ID).y / 1.1 };
		b2Body_SetLinearVelocity(ID, velocityVec);
	}

	if (timetillnextspot > 0) {
		timetillnextspot -= deltaTime;
	}

	if (travelling == false && timetillnextspot <= 0) {
		//LAYMINE FUNCTION HERE
		target.x = GetRandom(100, SCREEN_WIDTH-100);
		target.y = GetRandom(100, SCREEN_HEIGHT-100);
		travelling = true;
	}

	if (travelling == true) {
		m_position.x = b2Body_GetPosition(ID).x;
		m_position.y = b2Body_GetPosition(ID).y;
		angle = atan2(target.y - m_position.y, target.x - m_position.x);
		if (speed < 100) {
			speed += 2;
		}
		else {
			speed = 100;
		}

		if (TimerPostCollide > 0) {
			TimerPostCollide -= 2 * deltaTime;
		}
		else {
			if (offsetvelocity.x != 0) {
				offsetvelocity.x = 0;
				offsetvelocity.y = 0;
			}
			m_pSprite->SetRedTint(1.0f);
		}
		velocity.x = (speed * (cos(angle))) + (offsetvelocity.x * (TimerPostCollide / 2));
		velocity.y = (speed * (sin(angle))) + (offsetvelocity.y * (TimerPostCollide / 2));


		b2Vec2 velocityVec = { velocity.x, velocity.y };
		b2Body_SetLinearVelocity(ID, velocityVec);
		
	}
	m_pSprite->SetAngle(-angle * (180 / M_PI) - 90);
	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSprite->Process(deltaTime);
};
void
Minelayer::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};



void 
Minelayer::ProcessDamageCollision(b2BodyId collidingwith) {
	try {//attempt player damage
		PlayerObject* address = reinterpret_cast<PlayerObject*>(b2Body_GetUserData(collidingwith));
		if (address->CanDamage()) {
			health -= address->getDamage();
			if (address->CanHeal()) {
				address->AddHealth(bloodback, (health <= 0));//add health, and if enemy is dead (true) then dont lose momentum
			}

			if (health <= 0) {
				m_bAlive = false;
			}
		}
		else if (address->CanTakeDamage() && travelling == true) {//if travelling and not laying mine then damage players
			address->takedamage(damage);
		}

	}
	catch (...) {
	}

	if (travelling == false) {//if hit while laying mine
		target.x = GetRandom(100, SCREEN_WIDTH - 100);//pick new spot and flee
		target.y = GetRandom(100, SCREEN_HEIGHT - 100);
		travelling = true;
	}
	float angle = atan2(b2Body_GetLocalCenterOfMass(collidingwith).y - m_position.y, b2Body_GetLocalCenterOfMass(collidingwith).x - m_position.x);
	offsetvelocity.x += (b2Body_GetLinearVelocity(collidingwith).x * (cos(angle))) / 3;
	offsetvelocity.y += (b2Body_GetLinearVelocity(collidingwith).y * (sin(angle))) / 3;
	m_pSprite->SetRedTint(0.0f);
	TimerPostCollide = 3;
}


void Minelayer::ProcessCollision(b2BodyId collidingwith) {
	float angle = atan2(b2Body_GetLocalCenterOfMass(collidingwith).y - m_position.y, b2Body_GetLocalCenterOfMass(collidingwith).x - m_position.x);
	b2Vec2 OffsetForce = { 0,0 };
	OffsetForce.x -= (b2Body_GetLinearVelocity(collidingwith).x * (cos(angle)));
	OffsetForce.y -= (b2Body_GetLinearVelocity(collidingwith).y * (sin(angle)));
	b2Body_ApplyForce(collidingwith, OffsetForce, b2Body_GetLocalCenterOfMass(collidingwith), true);
}