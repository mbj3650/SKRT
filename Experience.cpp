// COMP710 JESSE
// This include:
#include "Experience.h"
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
#include <stdexcept>
float Experience::sm_fBoundaryWidth = 0.0f;
float Experience::sm_fBoundaryHeight = 0.0f;
Experience::Experience()
	:speed(10.0f),
	isColliding(false)
{

};
Experience::~Experience()
{
	//delete m_pSprite;
	//m_pSprite = 0;
};

bool
Experience::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position, float experiencetodrop)
{
	type = 50;
	Experiencesize = experiencetodrop;
	m_pPlayer = playerAddress;
	m_pSprite = renderer.CreateSprite("..\\assets\\exp.png");
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	health = 1;
	m_pSprite->SetBlueTint(0.0f);
	m_pSprite->SetRedTint(0.0f);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_pSprite->SetScale(0.05f* experiencetodrop/2);

	//CREATE BODY FOR THE WORLD TO USE AS SHAPE REFERENCE
	b2BodyDef Exp = b2DefaultBodyDef();

	Exp.position = position;
	ID = b2CreateBody(WorldID, &Exp);
	b2Body_SetType(ID, b2_dynamicBody);
	b2Body_SetUserData(ID, this);
	b2Polygon box = b2MakeBox(experiencetodrop, experiencetodrop);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.1f;
	Filter
	shapeDef.filter.categoryBits = PlayerObject;
	b2ShapeId shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);

	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	angle = 0;
	b2Body_SetAwake(ID, false);
	return true;
	b2Shape_EnableContactEvents(shapeId, true);
	b2Vec2 speedVec = { speed * (cos(m_pSprite->GetAngle())) , speed * (sin(m_pSprite->GetAngle())) };
	b2Body_ApplyForceToCenter(ID, speedVec, true);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
};



float Experience::getExperienceAngle() {
	return m_pSprite->GetAngle();
}


int Experience::getSize() {
	return Experiencesize;
}

float Experience::GetRadius() {
	return m_pSprite->GetWidth() / 2;
}

void
Experience::Process(float deltaTime)
{
	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	angle = atan2(b2Body_GetPosition(m_pPlayer).y - m_position.y, b2Body_GetPosition(m_pPlayer).x - m_position.x);
	velocity.x = (speed * (cos(angle)));
	velocity.y = (speed * (sin(angle)));
	if (isColliding) {
		isColliding = false;
	}
	double distance =
		sqrt(
			pow(((b2Body_GetPosition(ID).x) - (b2Body_GetPosition(m_pPlayer).x)), 2)
			+ pow(((b2Body_GetPosition(ID).y) - (b2Body_GetPosition(m_pPlayer).y)), 2)
		) - reinterpret_cast<PlayerObject*>(b2Body_GetUserData(m_pPlayer))->GetRadius() - GetRadius();//get distance to player
		b2Vec2 velocityVec = { velocity.x * (100/distance), velocity.y * (100 / distance) };
		//b2Body_SetLinearVelocity(ID, velocityVec);

	
	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSprite->Process(deltaTime);
};

void Experience::ProcessDamageCollision(b2BodyId collidingwith) {
	m_bAlive = false;
}

void Experience::ProcessCollision(b2BodyId collidingwith) {
	
}

void
Experience::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};

Vector2& Experience::Position
()
{
	return m_position;
}

void
Experience::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}

void
Experience::Kill() {

}

bool Experience::isAlive() {
	return m_bAlive;
}



void Experience::DebugDraw
()
{
	ImGui::InputFloat2("Position", reinterpret_cast<float*>(&m_position));
	ImGui::InputFloat2("VelocityX", reinterpret_cast<float*>(&velocity));
	ImGui::Text("Lowerbound (%f, %f)", m_boundaryLow.x, m_boundaryLow.y);
	ImGui::Text("Upperbound (%f, %f)", m_boundaryHigh.x, m_boundaryHigh.y);
}