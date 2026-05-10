// COMP710 JESSE
// This include:
#include "Bigboss.h"

// Local includes:
#include "EnemyBase.h"
#include "renderer.h"
#include "inlinehelpers.h"
#include "PlayerObject.h"
#include "sprite.h"
// Library includes:
#include <cassert>
#include <iostream>


Bigboss::Bigboss()
{

};
Bigboss::~Bigboss()
{
	std::cout << "DELETED SLOWER SPRITE\n";
	delete m_pSprite;
	m_pSprite = 0;
	delete m_pBooster;
	m_pBooster = 0;
};

bool Bigboss::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position)
{
	type = 222;

	experiencetodrop = 10;
	m_pPlayer = playerAddress;
	TimerPostCollide = 0;
	inbounds = false;
	attack = -1;
	//sprites
	m_pSprite = renderer.CreateSprite("..\\assets\\enemies\\bigboss.png");
	m_pSprite->SetScale(0.3);

	m_pSaws = renderer.CreateSprite("..\\assets\\enemies\\shield.png");
	m_pSaws->SetScale(0.35);

	//pointer arrow info
	m_pBooster = renderer.CreateSprite("..\\assets\\booster.png");
	m_pBooster->SetGreenTint(1.0f);
	m_pBooster->SetScale(0.3);
	m_pBooster->SetBlueTint(1.0f);

	radius = m_pSprite->GetWidth() / 2;

	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	SCREEN_WIDTH = renderer.GetWidth();
	SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	damage = 10;
	health = 300;
	speed = GetRandom(130, 180);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);




	//CREATE BODY FOR THE WORLD TO USE AS SHAPE REFERENCE
	b2BodyDef WorldObj = b2DefaultBodyDef();
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
	cooldown = 2;
	ID = b2CreateBody(WorldID, &WorldObj);
	b2Body_SetType(ID, b2_dynamicBody);
	b2Body_SetUserData(ID, this);
	b2Polygon box = b2MakeRoundedBox(15, 15, 5.0f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 20.0f;
	shapeDef.friction = 10000.0f;


	shapeDef.filter.categoryBits = 0x2222;//i am
	shapeDef.filter.maskBits = 0x0001 | 0x0002 | 0x1022 | 0x0022;//i collide with enemies and player

	shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);

	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	b2Body_SetAwake(ID, true);
	b2Shape_EnableContactEvents(shapeId, true);
	b2Vec2 speedVec = { speed * (cos(m_pSprite->GetAngle())) , speed * (sin(m_pSprite->GetAngle())) };
	b2Body_SetLinearVelocity(ID, speedVec);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;


}

void
Bigboss::Process(float deltaTime)
{
	m_position.x = b2Body_GetPosition(ID).x;//position set to box2d position
	m_position.y = b2Body_GetPosition(ID).y;


	//IF ATTACK UNCHOSEN
	if (attack == -1) {//-1 is default chasing ai
		m_pSprite->SetBlueTint(1.0f);
		m_pSprite->SetRedTint(1.0f);
		m_pSprite->SetGreenTint(1.0f);
		if (cooldown <= 0) {//IF NOT ON COOLDOWN
			attack = GetRandom(1,3);//PICK AN ATTACK, 1 is the summon one, sowe want it to be lesscommon
			if (attack != 1) {
				attack = 2;//think of 2 as the "charge attack" ID
			}
			std::cout << attack << "\n";
			if (attack == 3) {
				cooldown = 10;
			}
			else {
				cooldown = 5;//GO ON COOLDOWN
			}
			
			chargeup = 2;
		}
		else {//REDUCE COOLDOWN TIMER
			cooldown -= deltaTime;//decrease cooldowns between attacks
			if (chargingtimer <= 0) {
				//DO REGULAR CHASE ATTACK
				angle = atan2(b2Body_GetPosition(m_pPlayer).y - m_position.y, b2Body_GetPosition(m_pPlayer).x - m_position.x);//point at player
				b2Shape_SetDensity(shapeId, 20, true);
				velocity.x = (speed * (cos(angle))) + (offsetvelocity.x * (TimerPostCollide / 2));//SET VELOCITY TO FOLLOW PLAYER WHILE NO ATTACK
				velocity.y = (speed * (sin(angle))) + (offsetvelocity.y * (TimerPostCollide / 2));
				velocityVec = { velocity.x, velocity.y };
			}
			else {
				chargingtimer -= deltaTime;
			}

		}

		if (TimerPostCollide > 0) {//post collision timer decrease
			TimerPostCollide -= 2 * deltaTime;
		}
		else {
			if (offsetvelocity.x != 0) {
				offsetvelocity.x = 0;
				offsetvelocity.y = 0;
			}
			//m_pSprite->SetBlueTint(1.0f);
		}

	}

	//CHECK IF INBOUNDS
	if (!inbounds) {
		if ((m_pSprite->GetX() + radius < m_boundaryHigh.x && m_pSprite->GetX() - radius > m_boundaryLow.x) && (m_pSprite->GetY() + radius < m_boundaryHigh.y && m_pSprite->GetY() - radius > m_boundaryLow.y)) {//if entered screen
			inbounds = true;
			m_pSprite->SetBlueTint(0);
			m_pSprite->SetGreenTint(0);
		}
	}




	//ATTACK SELECTION

	if (attack == 1) {
		m_pSprite->SetBlueTint(0.2);
		m_pSprite->SetGreenTint(0.7);
		if (chargeup > 0) {//windup time
			chargeup -= deltaTime;
			angle = 55;//point at player
			velocity.y -= deltaTime *2* (velocity.y);
			velocity.x -= deltaTime * 2 * (velocity.x);
			
			//set speed
		}
		else {
			summonfriends = true;
			b2Shape_SetDensity(shapeId, 2000, true);
			chargeup = 2;//reset cooldown
			attack = -1;
		}
	}


	//ATTACK SELECTION

	if (attack == 2) {
		m_pSprite->SetBlueTint(0.7);
		m_pSprite->SetGreenTint(0.2);
		if (chargeup > 0) {//windup time
			chargeup -= 2*deltaTime;
			m_pBooster->SetScale(0.2 * (1 - chargeup / 2));
			angle = atan2(b2Body_GetPosition(m_pPlayer).y - m_position.y, b2Body_GetPosition(m_pPlayer).x - m_position.x);//point at player
			//set speed
		}
		else {
			//set speed
			velocity.x = (speed * 100 * (cos(angle))) + (offsetvelocity.x * (TimerPostCollide / 2));//SET VELOCITY TO FOLLOW PLAYER WHILE NO ATTACK
			velocity.y = (speed * 100 * (sin(angle))) + (offsetvelocity.y * (TimerPostCollide / 2));
			b2Shape_SetDensity(shapeId, 2000, true);
			chargeup = 2;//reset cooldown
			chargingtimer = 3;
			attack = -1;
		}
	}



	//IF INBOUNDS, CHECK IF NEEDS TO BOUNCE OFF WALL
	if (inbounds) {
		//bounce off edges
		if (b2Body_GetPosition(ID).x + (radius) > sm_fBoundaryWidth && velocity.x > 0)
		{
			velocity.x *= -1.0f;
		}
		else if (b2Body_GetPosition(ID).x - (radius) < 0 && velocity.x < 0)
		{
			velocity.x *= -1.0f;

		}
		else if (b2Body_GetPosition(ID).y + (radius) > sm_fBoundaryHeight && velocity.y > 0)
		{
			velocity.y *= -1.0f;
		}
		else if (b2Body_GetPosition(ID).y - (radius) < 0 && velocity.y < 0)
		{
			velocity.y *= -1.0f;
		}
	}

	if (chargeup > 0 && attack == 2) {//IF CHARGING AND ATTACK IS CHARGING
		velocityVec = { 0, 0 };
		b2Body_SetLinearVelocity(ID, velocityVec);
	}
	else {
		velocityVec = { velocity.x, velocity.y };
		b2Body_SetLinearVelocity(ID, velocityVec);
	}

	//sprite appearance code
	float aimangle = (180.0 / M_PI) * -angle;//sprite stuff;//make tracer follow mouse
	aimangle += 180;//make arrow point opposite too mouse
	float drawangle = -angle * (180 / M_PI) - 90;

	if (IFrames > 0) {//timer tick down animation
		IFrames -= deltaTime * 2;
		int Framelogic = IFrames * 10;
		if (Framelogic % 2 == 0) {
			m_pSprite->SetAlpha(1.0f);//flashing animation
		}
		else {
			m_pSprite->SetAlpha(0.0f);
		}
	}


	m_pSprite->SetAngle(drawangle);
	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSprite->Process(deltaTime);

	m_pBooster->SetAngle(drawangle);
	m_pBooster->SetX((static_cast<int>(b2Body_GetPosition(ID).x)) - (100 * (cos((M_PI / 180) * -aimangle) * (1 - chargeup / 2))));
	m_pBooster->SetY((static_cast<int>(b2Body_GetPosition(ID).y)) - (100 * (sin((M_PI / 180) * -aimangle) * (1 - chargeup / 2))));
	m_pBooster->Process(deltaTime);


	m_pSaws->SetAngle(m_pSaws->GetAngle() + deltaTime * 360);
	m_pSaws->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSaws->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSaws->Process(deltaTime);
};
void
Bigboss::Draw(Renderer& renderer)
{

	
	
	if (chargeup > 0 && attack == 1) {
		m_pSaws->Draw(renderer);
	}
	if (chargeup > 0 && attack == 2) {
		m_pBooster->Draw(renderer);
	}
	m_pSprite->Draw(renderer);
};




void Bigboss::ProcessDamageCollision(b2BodyId collidingwith) {//player damage
	try {//attempt player damage
		PlayerObject* address = reinterpret_cast<PlayerObject*>(b2Body_GetUserData(collidingwith));
		std::cout << "Got address\n";
		std::cout << "candamge : " << address->CanDamage() << "\n";
		if (chargingtimer <= 0 && attack == -1) {//if not charging and attack is -1
			if (address->CanDamage()) {
				std::cout << "ddamage ow\n";
				health -= address->getDamage();
				IFrames = 2;//boss cant take damage again
				if (address->CanHeal()) {
					address->AddHealth(bloodback, (health <= 0));//add health, and if enemy is dead (true) then dont lose momentum
				}
				if (needssound.empty() && TimerPostCollide <= 0) {//play hit sound
					needssound.push_back(1);
				}
				if (health <= 0) {
					m_bAlive = false;
				}
			}
			else if (address->CanTakeDamage()) {//if player is too slow then damage them
				if (chargingtimer > 0) {
					address->BigHit(ID);
				}
				address->takedamage(damage);
			}
		}
		else {//damage player if charging regardless of speed
			address->BigHit(ID);
		address->takedamage(damage);
		}
		

	}
	catch (...) {
	}
	//get knocked away from collision

	float angle = atan2(b2Body_GetLocalCenterOfMass(collidingwith).y - m_position.y, b2Body_GetLocalCenterOfMass(collidingwith).x - m_position.x);
	offsetvelocity.x += (b2Body_GetLinearVelocity(collidingwith).x * (cos(angle))) / 3;
	offsetvelocity.y += (b2Body_GetLinearVelocity(collidingwith).y * (sin(angle))) / 3;
	TimerPostCollide = 3;
}