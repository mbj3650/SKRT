// COMP710 JESSE
// This include:
#include "EnemyBase.h"
#include "PlayerObject.h"
#include <conio.h>
// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
#include "inlinehelpers.h"
#include "lib/imgui/imgui.h"
#include <box2d.h>
float EnemyBase::sm_fBoundaryWidth = 0.0f;
float EnemyBase::sm_fBoundaryHeight = 0.0f;
EnemyBase::EnemyBase()
	:speed(100.0f),
	damage(4.0f),
	bloodback(1)
{

};
EnemyBase::~EnemyBase()
{
	m_pSprite = 0;
};

bool
EnemyBase::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position)
{
	type = 100;//defaut enemy type
	experiencetodrop = GetRandom(1, 5);//drop a random amount of exp
	m_pPlayer = playerAddress;
	m_pSprite = renderer.CreateSprite("..\\assets\\enemies\\demon.png");
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	SCREEN_WIDTH = renderer.GetWidth();
	SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	health = 70;//needs 4 hits to kill until later upgrades
	speed = GetRandom(80, 100);//randomize the speed a bit
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_pSprite->SetScale(0.1f);//sprite
	
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


	shapeDef.filter.categoryBits = 0x0002;//i am
	shapeDef.filter.maskBits = 0x0001 | 0x0002;//i collide with

	shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);

	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	angle = 0;
	b2Body_SetAwake(ID, true);
	return true;
	b2Shape_EnableContactEvents(shapeId, true);
	//set initial velocity
	b2Vec2 speedVec = { speed * (cos(m_pSprite->GetAngle())) , speed * (sin(m_pSprite->GetAngle())) };
	b2Body_SetLinearVelocity(ID, speedVec);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}
bool EnemyBase::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID)//for subclasses
{
	return false;
}
;

bool EnemyBase::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position, float experiencetodrop)//for subclasses
{
	return false;
}
;

//gets
float EnemyBase::getEnemyBaseAngle() {
	return m_pSprite->GetAngle();
}

int EnemyBase::getSize() {
	return EnemyBasesize;
}

float EnemyBase::GetRadius() {
	return m_pSprite->GetWidth()/2;
}

void
EnemyBase::Process(float deltaTime)
{
	m_position.x = b2Body_GetPosition(ID).x;//position set to box2d position
	m_position.y = b2Body_GetPosition(ID).y;
	angle = atan2(b2Body_GetPosition(m_pPlayer).y - m_position.y, b2Body_GetPosition(m_pPlayer).x - m_position.x);//point at player
	if (speed < 100) {//speed t oreach 100 if possible
		speed += 2;
	}
	else {
		speed = 100;
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
	//slowly remove offset speed from main velocity
		velocity.x = (speed * (cos(angle))) + (offsetvelocity.x * (TimerPostCollide / 2));
		velocity.y = (speed * (sin(angle))) + (offsetvelocity.y * (TimerPostCollide / 2));

		//set speed
	b2Vec2 velocityVec = { velocity.x, velocity.y };
	b2Body_SetLinearVelocity(ID, velocityVec);
	//sprite appearance functions
	m_pSprite->SetAngle(-angle * (180 / M_PI) - 90);
	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSprite->Process(deltaTime);
};

void EnemyBase::ProcessCollision(b2BodyId collidingwith) {//doesnt do much otehr than just dont merge into it (deprecated due to maskbits but keeping just in case)
	float angle = atan2(b2Body_GetLocalCenterOfMass(collidingwith).y - m_position.y, b2Body_GetLocalCenterOfMass(collidingwith).x - m_position.x);
	b2Vec2 OffsetForce = {0,0};
	OffsetForce.x -= (b2Body_GetLinearVelocity(collidingwith).x * (cos(angle)));
	OffsetForce.y -= (b2Body_GetLinearVelocity(collidingwith).y * (sin(angle)));
	b2Body_ApplyForce(collidingwith, OffsetForce, b2Body_GetLocalCenterOfMass(collidingwith), true);
}

void EnemyBase::ProcessDamageCollision(b2BodyId collidingwith) {//player damage
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
		else if (address->CanTakeDamage()){//if player is too slow then damage them
			address->takedamage(damage);
		}
		
	}
	catch (...) {
	}
	//get knocked away from collision

	float angle = atan2(b2Body_GetLocalCenterOfMass(collidingwith).y - m_position.y, b2Body_GetLocalCenterOfMass(collidingwith).x - m_position.x);
	offsetvelocity.x += (b2Body_GetLinearVelocity(collidingwith).x * (cos(angle)))/3;
	offsetvelocity.y += (b2Body_GetLinearVelocity(collidingwith).y * (sin(angle))) / 3;
	m_pSprite->SetBlueTint(0.0f);
	TimerPostCollide = 3;
}

void
EnemyBase::Draw(Renderer& renderer)
{//draw
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};

Vector2& EnemyBase::Position
()
{
	return m_position;
}

void
EnemyBase::ComputeBounds(int width, int height)
{//bounds
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}

void
EnemyBase::Kill() {

}

bool EnemyBase::isAlive() {
	return m_bAlive;
}



void EnemyBase::DebugDraw//
()
{
	ImGui::Text("Type %d",type);
	ImGui::Text("Health %f", health);
	ImGui::Text("Damage %f", damage);
	ImGui::InputFloat2("Position", reinterpret_cast<float*>(&m_position));
	ImGui::InputFloat2("VelocityX", reinterpret_cast<float*>(&velocity));
	ImGui::Text("Lowerbound (%f, %f)", m_boundaryLow.x, m_boundaryLow.y);
	ImGui::Text("Upperbound (%f, %f)", m_boundaryHigh.x, m_boundaryHigh.y);
}