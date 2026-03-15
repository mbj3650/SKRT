#include "EntityBall.h"
// COMP710 GP Framework 2022
// This include:
#include "ball.h"
// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "inlinehelpers.h"
// Library includes:
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "lib/imgui/imgui.h"
// Static members:
EntityBall::EntityBall()
	: Goodbad(0)
{
}
EntityBall::~EntityBall()
{
	delete m_pSprite;
	m_pSprite = 0;
}
bool
EntityBall::Initialise(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("..\\assets\\ball.png");
	RandomiseEntitySize();
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_position.x = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_WIDTH - EDGE_LIMIT));
	m_position.y = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_HEIGHT - EDGE_LIMIT));
	m_velocity.x = GetRandomPercentage() * MAX_SPEED * GetPositiveOrNegative();
	m_velocity.y = GetRandomPercentage() * MAX_SPEED * GetPositiveOrNegative();

	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

float EntityBall::GetRadius() {
	return radius/2;
}

float EntityBall::GetGB() {
	return Goodbad;
}

void EntityBall::SetGood
()
{
	Goodbad = 1;
	m_pSprite->SetRedTint(0);
	m_pSprite->SetGreenTint(1);
	m_pSprite->SetBlueTint(0);
}
void EntityBall::SetBad
()
{
	Goodbad = 2;
	m_pSprite->SetRedTint(1);
	m_pSprite->SetGreenTint(0);
	m_pSprite->SetBlueTint(0);
}
void EntityBall::SetNeutral
()
{
	m_pSprite->SetRedTint(0.5f);
	m_pSprite->SetGreenTint(0.5f);
	m_pSprite->SetBlueTint(0.5f);
}
void EntityBall::RandomiseEntitySize
()
{
	float scale = GetRandomPercentage();
	scale *= 0.5f;
	m_pSprite->SetScale(scale);
	radius = (m_pSprite->GetWidth());
}

void EntityBall::UpdateScale(float scale)
{
	m_pSprite->SetScale(m_pSprite->GetScale() * scale);
	radius = (m_pSprite->GetWidth());
}