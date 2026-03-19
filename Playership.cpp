// COMP710 JESSE
// This include:
#include "Playership.h"

// Local includes:
#include "renderer.h"
#include "inputsystem.h"
#include "sprite.h"
// Library includes:
#include <cassert>
float Playership::sm_fBoundaryWidth = 0.0f;
float Playership::sm_fBoundaryHeight = 0.0f;

Playership::Playership()
{

};
Playership::~Playership()
{
	delete m_pSprite;
	m_pSprite = 0;
};

bool
Playership::Initialise(Renderer& renderer)
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
	m_position.x = (SCREEN_WIDTH / 2);
	m_position.y = SCREEN_HEIGHT /2;
	angle = 0;
	//ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	radius = m_pSprite->GetWidth();
	return true;
};

void
Playership::Process(float deltaTime, InputSystem& inputSystem)
{
	Vector2 mouse = inputSystem.GetMousePosition();
	angle = atan2( mouse.y- m_position.y ,  mouse.x- m_position.x );

	angle = (180 / M_PI) * -angle;
	angle += 270;
	m_pSprite->SetX(static_cast<int>(m_position.x));
	m_pSprite->SetY(static_cast<int>(m_position.y));
	m_pSprite->SetAngle(angle);
};
void
Playership::Draw(Renderer& renderer)
{

		m_pSprite->Draw(renderer);
	
};

Vector2 Playership::Position() {
	return m_position;
}

float Playership::GetShipAngle() {
	return m_pSprite->GetAngle();
}

