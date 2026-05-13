// COMP710 JESSE
// This include:
#include "Slower.h"

// Local includes:
#include "renderer.h"

// Library includes:
#include <cassert>
#include "sprite.h"
#include "AnimatedSprite.h"
#include "inlinehelpers.h"
#include "PlayerObject.h"
#include "particleemitter.h"
#include <iostream>
Slower::Slower()

{

};
Slower::~Slower()
{
	if (b2Body_IsValid(ID)) {
		b2DestroyBody(ID);//destroy impact body
	}
	std::cout << "DELETED SLOWER SPRITE\n";
	delete m_pSprite;
	delete m_pASprite;
	delete Radiation;
	delete m_pCircle;
	m_pCircle = 0;
	m_pASprite = 0;
	m_pSprite = 0;
	Radiation = 0;
};

bool
Slower::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position)
{
	type = 152;

	experiencetodrop = GetRandom(4, 8);
	m_pPlayer = playerAddress;
	TimerPostCollide = 0;


	//sprites
	m_pSprite = renderer.CreateSprite("..\\assets\\enemies\\corezone.png");
	m_pCircle = renderer.CreateSprite("..\\assets\\enemies\\slowzone.png");
	m_pASprite = renderer.CreateAnimatedSprite("..\\assets\\enemies\\demoncore_anim.png");

	m_pSprite->SetRedTint(0);
	m_pSprite->SetBlueTint(0);
	m_pSprite->SetAlpha(0);
	m_pSprite->SetScale(4);

	m_pCircle->SetRedTint(0);
	m_pCircle->SetBlueTint(0);
	m_pCircle->SetScale(1);
	m_pCircle->SetAlpha(0);

	m_pASprite->SetupFrames(64, 64);
	m_pASprite->SetFrameDuration(0.1);
	m_pASprite->SetLooping(true);
	m_pASprite->SetScale(0.1f * 307 / m_pASprite->GetWidth());



	//particles
	float defaultcolor[3] = { 0.5,1,0.2 };//Radiation Particle is green
	std::string particle = "..\\assets\\enemies\\radiation.png";//tracer to tell drags

	Radiation = new ParticleEmitter();
	Radiation->Initialise(renderer, particle.c_str(), 0.1, 0.3, 1200, defaultcolor, 0, 360, 3);
	Radiation->turnon();


	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pASprite->GetWidth();
	SCREEN_WIDTH = renderer.GetWidth();
	SCREEN_HEIGHT = renderer.GetHeight();
	travelling = true;//immeadiately begin looking for a spot to travel to
	m_bAlive = true;
	damage = 2;
	health = 60;
	speed = GetRandom(60, 180);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);




	needsmine = false;
	//CREATE BODY FOR THE WORLD TO USE AS SHAPE REFERENCE
	b2BodyDef WorldObj = b2DefaultBodyDef();
	if (position.x == -9999) {
		int sidetospawn = rand() % 4;//pick a random side to spawn from
		switch (sidetospawn) {
		case 1:
			WorldObj.position.x = 0 - m_pASprite->GetWidth();
			WorldObj.position.y = static_cast<float>(GetRandom(0, SCREEN_HEIGHT));
			//spawn on left anywhere
			break;
		case 2:
			WorldObj.position.x = static_cast<float>(SCREEN_WIDTH) + m_pASprite->GetWidth();
			WorldObj.position.y = static_cast<float>(GetRandom(0, SCREEN_HEIGHT));
			//spawn on right anywhere
			break;
		case 3:
			WorldObj.position.x = static_cast<float>(GetRandom(0, SCREEN_WIDTH));
			WorldObj.position.y = static_cast<float>(SCREEN_HEIGHT) + m_pASprite->GetHeight();
			//spawn on bottom anywhere
			break;
		default:
			WorldObj.position.x = static_cast<float>(GetRandom(0, SCREEN_WIDTH));
			WorldObj.position.y = 0 - m_pASprite->GetHeight();
			//spawn top anywhere
			break;
		}
	}
	else {
		WorldObj.position.x = position.x;
		WorldObj.position.y = position.y;
	}
	cooldown = 2;
	ID = b2CreateBody(WorldID, &WorldObj);
	b2Body_SetType(ID, b2_dynamicBody);
	b2Body_SetUserData(ID, this);
	b2Polygon box = b2MakeRoundedBox(6, 6, 5.0f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.1f;


	shapeDef.filter.categoryBits = 0x1022;//i am
	shapeDef.filter.maskBits = 0x0001 | 0x0002 | 0x1022;//i collide with enemies and player

	shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);

	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	b2Body_SetAwake(ID, true);
	return true;
	b2Shape_EnableContactEvents(shapeId, true);
	b2Vec2 speedVec = { speed * (cos(m_pASprite->GetAngle())) , speed * (sin(m_pASprite->GetAngle())) };
	b2Body_SetLinearVelocity(ID, speedVec);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}


void Slower::PickNewSpot() {
	detonation = 0;//reset detonation timer
	travelling = true;//start moving
	m_pASprite->SetBlueTint(1);
	m_pASprite->SetRedTint(1);
}

void
Slower::Process(float deltaTime)
{

	target.x = b2Body_GetPosition(m_pPlayer).x;
	target.y = b2Body_GetPosition(m_pPlayer).y;
	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	//velocity is speed + the offset of being collided with

	angle = atan2(target.y - m_position.y, target.x - m_position.x);
	velocity.x = (speed * (cos(angle))) - (offsetvelocity.x * (TimerPostCollide) * (TimerPostCollide));
	velocity.y = (speed * (sin(angle))) - (offsetvelocity.y * (TimerPostCollide) * (TimerPostCollide));
	double distance =
		sqrt(
			pow(((b2Body_GetPosition(ID).x) - (target.x)), 2)
			+ pow(((b2Body_GetPosition(ID).y) - (target.y)), 2)
		) - m_pSprite->GetWidth() / 2;//get distance to player

	if (cooldown > 0) {//if on cooldown and cant detonate
		cooldown -= deltaTime;
	}
	else {
		if (distance < 6 && TimerPostCollide <= 0) {//if reached spot
			if (travelling == true) {//set travelling to false and stop moving
				travelling = false;
				detonation = 0.75;//also set cooldown
			}
		}
	}
	

	if (TimerPostCollide >= 0) {//if stunned
		travelling = true;//tells it not to pick a new spot since it needs to spin away
		TimerPostCollide -= deltaTime;//reduce timer
	}
	else {//if isnt stunned, then see if detonating

		if (detonation > 0) {//if currently detonating mine
			detonation -= deltaTime;//decrease timer
			if (!m_pASprite->IsAnimating()) {//if not yet animating
				playingsound = true;//tell game to play sound
			}
			m_pASprite->Animate();
			m_pASprite->SetLooping(true);
			m_pASprite->SetAngle(0);
			m_pSprite->SetAlpha(1-(detonation/0.75));
			if ((detonation < 0.25 && detonation > 0.20) || (detonation < 0.15 && detonation > 0.10) || (detonation < 0.05 && detonation > 0.00)) {
				m_pCircle->SetAlpha(0.25);
			}
			else if(m_pCircle->GetAlpha() == 0.25){
				m_pCircle->SetAlpha(0);
				if (m_pCircle->GetScale() < 4) {
					m_pCircle->SetScale(m_pCircle->GetScale() + 1.35);
				}
			}
			
			m_pASprite->SetBlueTint(1 * (detonation) / 0.75);
			m_pASprite->SetRedTint(1 * (detonation) / 0.75);
			m_pSprite->SetAngle(m_pSprite->GetAngle() + 10 * deltaTime * (1 / (detonation) * (1 / (detonation))));
			if (distance > 5) {
				b2Vec2 velocityVec = { velocity.x * 1.8, velocity.y * 1.8 };
				b2Body_SetLinearVelocity(ID, velocityVec);
			}
			else {
				b2Vec2 velocityVec = { velocity.x / 1.2, velocity.y / 1.2 };
				b2Body_SetLinearVelocity(ID, velocityVec);
			}
			
			
		}
	}


	if (travelling == false) {//if not travelling and hasnt been interrupted for 3 seconds
		if (detonation <= 0) {//if isnt waiting to place mine
			//CHECK IF PLAYER IS CLOSE ENOUGH AND SLOW THEM
			PlayerObject* address = reinterpret_cast<PlayerObject*>(b2Body_GetUserData(m_pPlayer));
			double distance =
				sqrt(
					pow(((b2Body_GetPosition(ID).x) - (target.x)), 2)
					+ pow(((b2Body_GetPosition(ID).y) - (target.y)), 2)
				) - (m_pSprite->GetWidth() / 2) - address->GetRadius();//get distance to player
			if (distance < 13) {
				address->slowdown();
			}
			cooldown = 2;
			PickNewSpot();
			
			m_pASprite->SetLooping(false);
			m_pASprite->StopAnimating();
			m_pASprite->Restart();
			m_pSprite->SetAlpha(0);
			m_pCircle->SetScale(1);
			m_pCircle->SetAlpha(0);
		}

	}
	else if (TimerPostCollide <= 0) {//if travelling and not collide and not detonating
		if (speed < 100) {//try to reach 100 speed if slower
			speed += 2;
		}
		else {
			speed = 100;
		}

		b2Vec2 velocityVec = { velocity.x, velocity.y };
		b2Body_SetLinearVelocity(ID, velocityVec);

	}
	if (!m_pASprite->IsAnimating()) {//if not animating
		m_pASprite->SetAngle(-angle * (180 / M_PI) - 90);
	}

	Radiation->SetParticlePosition(m_position);
	Radiation->Process(deltaTime);
	m_pASprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pASprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));

	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));

	m_pCircle->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pCircle->SetY(static_cast<int>(b2Body_GetPosition(ID).y));

	m_pASprite->Process(deltaTime);
};
void
Slower::Draw(Renderer& renderer)//draw
{

	if (m_bAlive)
	{
		m_pCircle->Draw(renderer);
		m_pSprite->Draw(renderer);
		m_pASprite->Draw(renderer);
		Radiation->Draw(renderer);
	}
};



void
Slower::ProcessDamageCollision(b2BodyId collidingwith) {
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
	float angle = atan2(b2Body_GetLocalCenterOfMass(collidingwith).y - m_position.y, b2Body_GetLocalCenterOfMass(collidingwith).x - m_position.x);
	offsetvelocity.x += (b2Body_GetLinearVelocity(collidingwith).x * (cos(angle))) / 5;
	offsetvelocity.y += (b2Body_GetLinearVelocity(collidingwith).y * (sin(angle))) / 5;
}


void Slower::ProcessCollision(b2BodyId collidingwith) {
	float angle = atan2(b2Body_GetLocalCenterOfMass(collidingwith).y - m_position.y, b2Body_GetLocalCenterOfMass(collidingwith).x - m_position.x);
	b2Vec2 OffsetForce = { 0,0 };
	OffsetForce.x -= (b2Body_GetLinearVelocity(collidingwith).x * (cos(angle)));
	OffsetForce.y -= (b2Body_GetLinearVelocity(collidingwith).y * (sin(angle)));
	b2Body_ApplyForce(collidingwith, OffsetForce, b2Body_GetLocalCenterOfMass(collidingwith), true);
}