// COMP710 JESSE
// This include:
#include "Tree.h"
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
#include <iostream>
float Tree::sm_fBoundaryWidth = 0.0f;
float Tree::sm_fBoundaryHeight = 0.0f;
Tree::Tree()
	:speed(100.0f),
	isColliding(false)
{

};
Tree::~Tree()
{

	std::cout << "DELETED Tree SPRITE\n";
	if (b2Body_IsValid(ID)) {
		b2DestroyBody(ID);//destroy impact body
	}
	delete m_pSprite;
	m_pSprite = 0;
};

bool
Tree::Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID)
{
	type = 111;
	damage = 0;
	m_pPlayer = playerAddress;
	m_pSprite = renderer.CreateSprite("..\\assets\\enemies\\pillar.png");
	const float MAX_SPEED = 0.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	health = 10000;
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_pSprite->SetScale(0.25);
	damage = 4;
	b2BodyDef WorldObj = b2DefaultBodyDef();
	WorldObj.position.x = static_cast<float>(GetRandom(SCREEN_WIDTH/10, SCREEN_WIDTH/1.1));
	WorldObj.position.y = static_cast<float>(GetRandom(SCREEN_HEIGHT / 10, SCREEN_HEIGHT / 1.1));

	//CREATE BODY FOR THE WORLD TO USE AS SHAPE REFERENCE
	ID = b2CreateBody(WorldID, &WorldObj);
	b2Body_SetType(ID, b2_dynamicBody);
	b2Body_SetUserData(ID, this);
	b2Polygon box = b2MakeRoundedBox(6, 12, 5.0f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 100000000000.0f;
	shapeDef.friction = 0.1f;


	shapeDef.filter.categoryBits = 0x0111;//i am
	shapeDef.filter.maskBits = 0x0001 | 0x0111;//i collide with

	shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);

	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;

	b2Body_SetAwake(ID, true);
	b2Shape_EnableContactEvents(shapeId, true);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
};




float Tree::GetRadius() {
	return m_pSprite->GetWidth() / 2;
}

void
Tree::Process(float deltaTime)
{
	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSprite->Process(deltaTime);
};

void Tree::ProcessDamageCollision(b2BodyId collidingwith) {
		PlayerObject* address = reinterpret_cast<PlayerObject*>(b2Body_GetUserData(collidingwith));
		address->Flip();
}

void Tree::ProcessCollision(b2BodyId collidingwith) {

}

void
Tree::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};

Vector2& Tree::Position
()
{
	return m_position;
}


bool Tree::isAlive() {
	return m_bAlive;
}

void
Tree::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}

void
Tree::Kill() {

}
