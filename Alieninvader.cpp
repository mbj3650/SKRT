// COMP710 JESSE
// This include:
#include "Alieninvader.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
#include "inlinehelpers.h"
int Alieninvader::madesofar = 0;
float Alieninvader::direction = 1;
bool Alieninvader::Hittingboundary = false;
Alieninvader* Alieninvader::List[55];
float Alieninvader::sm_fBoundaryWidth = 0.0f;
float Alieninvader::sm_fBoundaryHeight = 0.0f;
Alieninvader::Alieninvader()
{

};
Alieninvader::~Alieninvader()
{
	delete m_pSprite;
	m_pSprite = 0;
};

void
Alieninvader::ArrayInit(Alieninvader* replacement[]) {
	for (int i = 0; i < 55; i++) {
		List[i] = replacement[i];
	}
}


bool
Alieninvader::Initialise(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("..\\assets\\triangle.png");
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	m_pSprite->SetGreenTint(0.0f);
	m_pSprite->SetBlueTint(0.0f);
	m_pSprite->SetAngle(180.0f);
	m_pSprite->SetScale(0.1f);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_position.x = 500 + (((madesofar % 11) * m_pSprite->GetWidth()));
	m_position.y = 200 + ((madesofar/11) * m_pSprite->GetWidth());
	madesofar++;
	ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	radius = (m_pSprite->GetWidth());
	return true;
};

void Alieninvader::Kill() {
	m_bAlive = false;
}

void
Alieninvader::Process(float deltaTime)
{
	if (m_bAlive) {
		m_position.x += (0.05 * direction);
		if (m_position.x > (m_boundaryHigh.x))
		{
			m_position.x = m_boundaryHigh.x;
			direction *= -1;
			Rowdown();
		}
		else if (m_position.x < (m_boundaryLow.x))
		{
			m_position.x = m_boundaryLow.x;
			direction *= -1;
			Rowdown();
		}
		m_pSprite->SetX(static_cast<int>(m_position.x));
		m_pSprite->SetY(static_cast<int>(m_position.y));
		m_pSprite->Process(deltaTime);
	}
};
void
Alieninvader::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprite->Draw(renderer);
	}
};

void
Alieninvader::Rowdown() {
	for (int i = 0; i < 55; i++) {
		List[i]->m_position.y += m_pSprite->GetHeight();
	}
}

void
Alieninvader::DebugDraw() {

}

bool
Alieninvader::GetHitting() {
	if (Hittingboundary == true) {
		Hittingboundary = false;
		return true;
	}
	else {
		return false;
	}
}


void
Alieninvader::ComputeBounds(int width, int height)
{
	m_boundaryLow.x = (m_pSprite->GetWidth() / 2.0f);
	m_boundaryLow.y = (m_pSprite->GetHeight() / 2.0f);
	m_boundaryHigh.x = width - (m_pSprite->GetWidth() / 2.0f);
	m_boundaryHigh.y = height - (m_pSprite->GetHeight() / 2.0f);
}

Vector2& Alieninvader::Position
()
{
	return m_position;
}
float Alieninvader::GetRadius() {
	return radius;
}

bool Alieninvader::isAlive() {
	return m_bAlive;
}

void Alieninvader::Speedup() {
	direction = direction*1.01;
}