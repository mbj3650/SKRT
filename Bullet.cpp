// COMP710 JESSE
// This include:
#include "Bullet.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
// Static members:
float Bullet::sm_fBoundaryWidth = 0.0f;
float Bullet::sm_fBoundaryHeight = 0.0f;
Bullet::Bullet()
	:speed(0.1)
{

};
Bullet::~Bullet()
{
	delete m_pSprite;
	m_pSprite = 0;
};

bool
Bullet::Initialise(Vector2 Position, Renderer& renderer,float angle)
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
	m_pSprite->SetAngle(angle+90);
	
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_position.x = Position.x;
	m_position.y = Position.y;
	velocity.x = speed * (cos((M_PI / 180) * -m_pSprite->GetAngle()));
	velocity.y = speed * (sin((M_PI / 180) * -m_pSprite->GetAngle()));
	m_pSprite->SetScale(0.05);
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	radius = (m_pSprite->GetWidth());
	return true;
};

void
Bullet::Process(float deltaTime)
{
	if (m_bAlive)
	{
		//assert(0);
		if (m_position.y < m_boundaryLow.y) {
			m_position.y = m_boundaryHigh.y + velocity.y;
		}
		else if (m_position.y > m_boundaryHigh.y) {
			m_position.y = m_boundaryLow.y + velocity.y;
		}
		else if (m_position.x < m_boundaryLow.x) {
			m_position.x = m_boundaryHigh.x + velocity.x;
		}
		else if (m_position.x > m_boundaryHigh.x) {
			m_position.x = m_boundaryLow.x + velocity.x;
		}
			m_position.x += velocity.x;
			m_position.y += velocity.y;
			m_pSprite->SetX(static_cast<int>(m_position.x));
			m_pSprite->SetY(static_cast<int>(m_position.y));
			m_pSprite->Process(deltaTime);
			
	}

};


void
Bullet::Kill() {
	//this->m_bAlive = false;
	delete(this);
}

void
Bullet::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};
void
Bullet::DebugDraw() {

}

Vector2& Bullet::Position() {
	return m_position;
}

bool Bullet::isAlive() {
	return m_bAlive;
}
float Bullet::GetRadius() {
	return radius;
}


void
Bullet::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}