// COMP710 JESSE
// This include:
#include "Laserbullet.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
// Static members:
float Laserbullet::sm_fBoundaryWidth = 0.0f;
float Laserbullet::sm_fBoundaryHeight = 0.0f;
Laserbullet::Laserbullet()
{

};
Laserbullet::~Laserbullet()
{
	delete m_pSprite;
	m_pSprite = 0;
};

bool
Laserbullet::Initialise(Vector2 Position, Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("..\\assets\\ball.png");
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	m_pSprite->SetGreenTint(0.0f);
	m_pSprite->SetRedTint(0.0f);
	m_pSprite->SetScale(0.095f);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_position.x = Position.x;
	m_position.y = Position.y;
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	radius = (m_pSprite->GetWidth());
	return true;
};

void
Laserbullet::Process(float deltaTime)
{
	if (m_bAlive)
	{
		if (m_position.y < m_boundaryLow.y) {
			Kill();
			return;
		}
		else {
			m_position.y -= 0.45;
			m_pSprite->SetX(static_cast<int>(m_position.x));
			m_pSprite->SetY(static_cast<int>(m_position.y));
			m_pSprite->Process(deltaTime);
			//assert(0);
		}
	}
	
};


void
Laserbullet::Kill() {
	this->m_bAlive = false;
	//delete(this);
}

void
Laserbullet::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};
void
Laserbullet::DebugDraw() {

}

Vector2& Laserbullet::Position() {
	return m_position;
}

bool Laserbullet::isAlive() {
	return m_bAlive;
}
float Laserbullet::GetRadius() {
	return radius;
}


void
Laserbullet::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}