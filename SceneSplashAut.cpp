// COMP710 GP Framework 2025
// This include:
#include "SceneSplashAUT.h"
// Local includes:
#include "renderer.h"
#include "sprite.h"
// Library includes:
#include <cassert>
SceneSplashAUT::SceneSplashAUT()
	: AlphaDir(1)
	, Fade(0.00025)
{
}
SceneSplashAUT::~SceneSplashAUT()
{
	delete m_pCentre;
	m_pCentre = 0;
	delete title;
	title = 0;
}
bool SceneSplashAUT::Initialise(Renderer& renderer)
{
	title = renderer.CreateSprite("..\\assets\\title.png");
	m_pCentre = renderer.CreateSprite("..\\assets\\splash2.png");
	m_pCentre->SetScale(0.5f);
	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);
	m_pCentre->SetAlpha(0.0f);

	title->SetX(SCREEN_WIDTH / 2);
	title->SetY(SCREEN_HEIGHT / 2);
	title->SetAlpha(0.0f);

	return true;
}
void
SceneSplashAUT::Process(float deltaTime, InputSystem& inputSystem)
{
	totaltime += deltaTime;
	if (totaltime < 3) {
		float currentalpha = m_pCentre->GetAlpha();
		if (currentalpha >= 1) {
			AlphaDir = -1;
		}
		else if (currentalpha <= 0) {
			AlphaDir = 1;
		}
		m_pCentre->SetAlpha(currentalpha + (AlphaDir * Fade));
		m_pCentre->Process(deltaTime);
	}
	else {
		float currentalpha = title->GetAlpha();
		if (currentalpha >= 1) {
			AlphaDir = -1;
		}
		else if (currentalpha <= 0) {
			AlphaDir = 1;
		}
		title->SetAlpha(currentalpha + (AlphaDir * Fade));
		title->Process(deltaTime);
	}
	
}
void
SceneSplashAUT::Draw(Renderer& renderer)
{
	if (totaltime < 3) {
		m_pCentre->Draw(renderer);
	}
	else {
		title->Draw(renderer);
	}
}
void SceneSplashAUT::DebugDraw
()
{

}