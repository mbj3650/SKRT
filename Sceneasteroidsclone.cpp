// COMP710 JESSE
// This include:
#include "Sceneasteroidsclone.h"

// Local includes:
#include "renderer.h"
#include "scene.h"
#include "sprite.h"
#include "Bullet.h"
#include "Playership.h"
#include "Asteroid.h"
// Library includes:
#include <cassert>
#include "lib/imgui/imgui.h"
#include "inlinehelpers.h"

Sceneasteroidsclone::Sceneasteroidsclone()
	: m_pAsteroids{ 0 }
	, m_iShowCount(0)
	, m_pBullets { 0 }
	, cooldown(0)//time between shooting
{

};
Sceneasteroidsclone::~Sceneasteroidsclone()
{
	for (int k = 0; k < 100; ++k)
	{
		delete m_pAsteroids[k];
		m_pAsteroids[k] = 0;

	}
	for (int k = 0; k < 10; ++k)
	{
		delete m_pBullets[k];
		m_pBullets[k] = 0;
	}
	delete m_pShip;
	m_pShip = 0;
};

bool
Sceneasteroidsclone::Initialise(Renderer& renderer)
{
	storage = &renderer;
	m_iShowCount = 0;
	for (int i = 0; i < 20; i++) {
		CreateAsteroid(999,NULL,NULL,0);
		//m_iShowCount++;
	}
	m_pShip = new Playership();
	m_pShip->Initialise(renderer);
	// Always place one ball at the centre...
	
	return true;
};

void Sceneasteroidsclone::CreateAsteroid(float angle, float positionX,float positionY, float Size) {
	float AstAngle;
	for (int i = 0; i < 100; i++) {
		if (m_pAsteroids[i] == NULL) {
			m_pAsteroids[i] = new Asteroid();
			TotalAsteroids++;
			if (angle == 999) {
				AstAngle = 360 * GetRandomPercentage();
				m_pAsteroids[i]->Initialise(*storage, AstAngle, 0);
			}
			else {
				m_pAsteroids[i]->Initialise(*storage, angle, Size);
			}
			if (positionX != NULL) {
				m_pAsteroids[i]->Position().x = positionX;
				m_pAsteroids[i]->Position().y = positionY;
			}
			break;
		}
	}
	
}

void
Sceneasteroidsclone::Process(float deltatime,InputSystem& inputsystem)
{
	int result = inputsystem.GetMouseButtonState(SDL_BUTTON_LEFT);
	if (result == BS_PRESSED)
	{
		if (Shooting == false) {
			Shooting = true;
			for (int i = 0; i < 10; i++) {
				if (m_pBullets[i] == NULL) {
					m_pBullets[i] = new Bullet();
					m_pBullets[i]->Initialise(m_pShip->Position(), *storage, m_pShip->GetShipAngle());
					break;
				}
			}
		}
	}
	else if (result == BS_RELEASED)
	{
		if (Shooting == true) {
			Shooting = false;
		}
	}

	for (int i = 0; i < 10; i++) {
		if (m_pBullets[i] != NULL) {
			m_pBullets[i]->Process(deltatime);
		}
	}


	for (int i = 0; i < 100; i++) {
		if (m_pAsteroids[i] != NULL) {
			m_pAsteroids[i]->Process(deltatime);
		}
	}
	
		m_pShip->Process(deltatime, inputsystem);



		for (int i = 0; i < 10; i++) {
			if (m_pBullets[i] != NULL) {
				if (m_pBullets[i]->isAlive()) {
					m_pBullets[i]->Process(deltatime);
					for (int k = 0; k < 100; ++k)
					{
						if (m_pAsteroids[k] != NULL) {
							double distance =
								sqrt(
									pow(((m_pBullets[i]->Position().x) - (m_pAsteroids[k]->Position().x)), 2)
									+ pow(((m_pBullets[i]->Position().y) - (m_pAsteroids[k]->Position().y)), 2)
								) - m_pAsteroids[k]->GetRadius() - m_pBullets[i]->GetRadius();
							if (distance <= 0) {
								if (m_pAsteroids[k]->getSize() == 1) {
									m_pAsteroids[k] = 0;
									m_pBullets[i] = 0;
									TotalAsteroids--;
									if (TotalAsteroids < 20) {
										CreateAsteroid(999, NULL, NULL, 0);
									}
								}
								else {
									CreateAsteroid(m_pAsteroids[k]->getAsteroidAngle() + 90, m_pAsteroids[k]->Position().x, m_pAsteroids[k]->Position().y, m_pAsteroids[k]->getSize() - 1);
									CreateAsteroid(m_pAsteroids[k]->getAsteroidAngle() - 90, m_pAsteroids[k]->Position().x, m_pAsteroids[k]->Position().y, m_pAsteroids[k]->getSize() - 1);
									m_pAsteroids[k] = 0;
									m_pBullets[i] = 0;
									TotalAsteroids--;
								}
							
								break;
							}
						}
					}
				}
			}
		}

};
void
Sceneasteroidsclone::Draw(Renderer& renderer)
{
	for (int i = 0; i < 100; i++) {
		if (m_pAsteroids[i] != NULL) {
			m_pAsteroids[i]->Draw(renderer);
		}
	}

	for (int i = 0; i < 10; i++) {
		if (m_pBullets[i] != NULL) {
			m_pBullets[i]->Draw(renderer);
		}
	}

	m_pShip->Draw(renderer);
};



void Sceneasteroidsclone::DebugDraw
()
{
	ImGui::Text("Scene: Asteroids");
	ImGui::Text("Total Asteroids %d", TotalAsteroids);
	ImGui::Text("Angle %f", m_pShip->GetShipAngle());
	static int editBallNumber = 0;
}