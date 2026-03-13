// COMP710 GP Framework 2025
// This include:
#include "SceneSplashAUT.h"
// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
SceneSplashAUT::SceneSplashAUT()
	: m_pCentre(0)
	, AlphaDir(1)
	, Fade(0.00025)
{
}
SceneSplashAUT::~SceneSplashAUT()
{
	delete m_pCentre;
	m_pCentre = 0;
}
bool SceneSplashAUT::Initialise(Renderer& renderer)
{
	
	m_pCentre = renderer.CreateSprite("..\\assets\\splash.png");
	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);
	m_pCentre->SetAlpha(0.0f);
	return true;
}
void
SceneSplashAUT::Process(float deltaTime)
{
	float currentalpha = m_pCentre->GetAlpha();
	if (currentalpha >= 1){
		AlphaDir = -1;
	}
	m_pCentre->SetAlpha(currentalpha + (AlphaDir * Fade));
	m_pCentre->Process(deltaTime);
}
void
SceneSplashAUT::Draw(Renderer& renderer)
{
	renderer.SetClearColour(0, 0, 0);
	m_pCentre->Draw(renderer);
}