// COMP710 JESSE
// This include:
#include "Asteroid.h"
#include <conio.h>
// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
#include "inlinehelpers.h"
#include "lib/imgui/imgui.h"
float Asteroid::sm_fBoundaryWidth = 0.0f;
float Asteroid::sm_fBoundaryHeight = 0.0f;
Asteroid::Asteroid()
	:speed(100.0f)
{

};
Asteroid::~Asteroid()
{
	delete m_pSprite;
	m_pSprite = 0;
};

bool
Asteroid::Initialise(Renderer& renderer, float angle, int size)
{
	float radian = 3.1415926535897932 / 180 * angle;
	m_pSprite = renderer.CreateSprite("..\\assets\\asteroid.png");
	if (size == 0) {
		RandomiseSize();
	}
	else {
		asteroidsize = size;
		m_pSprite->SetScale(size * 0.25);
	}

	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	do {
		m_position.x = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_WIDTH - EDGE_LIMIT));
		m_position.y = static_cast<float>(GetRandom(EDGE_LIMIT, SCREEN_HEIGHT - EDGE_LIMIT));
	} while (((SCREEN_WIDTH / 2) - 300 < m_position.x) && ((SCREEN_WIDTH / 2) + 300 > m_position.x)
		&& ((SCREEN_HEIGHT / 2) - 300 < m_position.y) && ((SCREEN_HEIGHT / 2) + 300 > m_position.y)
		);
	
	m_pSprite->SetAngle(angle);
	velocity.x = speed * (cos(m_pSprite->GetAngle())) * (1.0f / this->asteroidsize);
	velocity.y = speed * (sin(m_pSprite->GetAngle())) * (1.0f / this->asteroidsize);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
};

float Asteroid::getAsteroidAngle() {
	return m_pSprite->GetAngle();
}

int Asteroid::getSize() {
	return asteroidsize;
}

float Asteroid::GetRadius() {
	return m_pSprite->GetWidth();
}

void
Asteroid::Process(float deltaTime)
{
	if (m_position.x >= (m_boundaryHigh.x + m_pSprite->GetWidth()))
	{
		m_position.x = m_boundaryLow.x- m_pSprite->GetWidth();
	}
	else if (m_position.x <= (m_boundaryLow.x - m_pSprite->GetWidth()))
	{
		m_position.x = m_boundaryHigh.x + m_pSprite->GetWidth();
	}
	if (m_position.y >= (m_boundaryHigh.y + m_pSprite->GetWidth()))
	{
		m_position.y = m_boundaryLow.y - m_pSprite->GetWidth();
	}
	else if (m_position.y <= (m_boundaryLow.y- m_pSprite->GetWidth()))
	{
		m_position.y = m_boundaryHigh.y + m_pSprite->GetWidth();
	}
	m_position.x += velocity.x * deltaTime;
	m_position.y += velocity.y * deltaTime;
	m_pSprite->SetX(static_cast<int>(m_position.x));
	m_pSprite->SetY(static_cast<int>(m_position.y));
	m_pSprite->Process(deltaTime);
};
void
Asteroid::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};

void Asteroid::RandomiseSize
()
{
	float size = (rand() % 3) + 1;//min 1 max 3
	asteroidsize = size;
	m_pSprite->SetScale(size *0.25);
}


Vector2& Asteroid::Position
()
{
	return m_position;
}

void
Asteroid::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}

void
Asteroid::Kill() {
	//this->m_bAlive = false;
	delete(this);
}


void Asteroid::DebugDraw
()
{
	ImGui::InputFloat2("Position", reinterpret_cast<float*>(&m_position));
	ImGui::InputFloat2("VelocityX", reinterpret_cast<float*>(&velocity));
	ImGui::Text("Size (%d)", this->asteroidsize);
	ImGui::Text("Lowerbound (%f, %f)", m_boundaryLow.x, m_boundaryLow.y);
	ImGui::Text("Upperbound (%f, %f)", m_boundaryHigh.x, m_boundaryHigh.y);
}