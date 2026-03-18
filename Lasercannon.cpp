// COMP710 JESSE
// This include:
#include "Lasercannon.h"
#include "inputsystem.h"
// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
float LaserCannon::sm_fBoundaryWidth = 0.0f;
float LaserCannon::sm_fBoundaryHeight = 0.0f;
LaserCannon::LaserCannon()
{
	delete m_pSprite;
	m_pSprite = 0;
};
LaserCannon::~LaserCannon()
{

};

bool
LaserCannon::Initialise(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("..\\assets\\triangle.png");
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	m_pSprite->SetRedTint(0.0f);
	m_pSprite->SetScale(0.075);
	m_pSprite->SetBlueTint(0.0f);
	//m_pSprite->SetAngle(180.0f);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_position.x = (SCREEN_WIDTH/2);
	m_position.y = SCREEN_HEIGHT-200- m_pSprite->GetWidth();
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	radius = m_pSprite->GetWidth();
	return true;
};

void
LaserCannon::Process(float deltaTime, InputSystem& inputSystem)
{
	if (m_bAlive) {
		if (inputSystem.GetKeyState(SDL_SCANCODE_A)) {
			if (m_position.x <= (m_boundaryLow.x))
			{
				m_position.x = m_boundaryLow.x;
			}
			else {
				if (inputSystem.GetKeyState(SDL_SCANCODE_RSHIFT) || inputSystem.GetKeyState(SDL_SCANCODE_LSHIFT)) {
					m_position.x -= (0.05);
				}
				m_position.x -= (0.05);
			}
		}
		else if (inputSystem.GetKeyState(SDL_SCANCODE_D)) {
			if (m_position.x >= (m_boundaryHigh.x))
			{
				m_position.x = m_boundaryHigh.x;
			}
			else {
				if (inputSystem.GetKeyState(SDL_SCANCODE_RSHIFT) || inputSystem.GetKeyState(SDL_SCANCODE_LSHIFT)) {
					m_position.x += (0.05);
				}
				m_position.x += (0.05);
			}
		}
		m_pSprite->SetX(static_cast<int>(m_position.x));
		m_pSprite->SetY(static_cast<int>(m_position.y));
		m_pSprite->Process(deltaTime);
	}
};
void
LaserCannon::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};

void
LaserCannon::DebugDraw() {

}

Vector2& LaserCannon::Position() {
	return m_position;
}

void
LaserCannon::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}

bool LaserCannon::isAlive() {
	return m_bAlive;
}

void LaserCannon::Kill() {
	this->m_bAlive = false;
}

float LaserCannon::GetRadius() {
	return radius;
}