// COMP710 Steffan Hooper
// This include:
#include "SceneSpaceInvadersClone.h"
// Local includes:
#include "renderer.h"
#include "lasercannon.h"
#include "Laserbullet.h"
#include "Alieninvader.h"
#include "inputsystem.h"
// Library includes:
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "lib/imgui/imgui.h"
SceneSpaceInvadersClone::SceneSpaceInvadersClone()
	: m_pInvaders{ 0 }
	, m_pLaser{ 0 }
	, m_iShowCount(0)
	, m_pCannon(0)
	, cooldown (0)//time between shooting
{
}
SceneSpaceInvadersClone::~SceneSpaceInvadersClone()
{
	for (int k = 0; k < 55; ++k)
	{
		delete m_pInvaders[k];
		m_pInvaders[k] = 0;
	}
}
bool
SceneSpaceInvadersClone::Initialise(Renderer& renderer)
{
	storage = &renderer;
	for (int k = 0; k < 55; ++k)
	{
		m_pInvaders[k] = new Alieninvader();
		m_pInvaders[k]->Initialise(renderer);
	}
	m_pInvaders[0]->ArrayInit(m_pInvaders);
	// Always place one ball at the centre...
	m_pCannon = new LaserCannon();
	m_pCannon->Initialise(renderer);
	m_iShowCount = 55;
	
	return true;
}
void SceneSpaceInvadersClone::CheckBulletCollissions()
{
}


void
SceneSpaceInvadersClone::Process(float deltaTime, InputSystem& inputSystem)
{
	if (m_pCannon->isAlive()) {
		if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE) || inputSystem.GetKeyState(SDL_SCANCODE_KP_SPACE)) {
			if (cooldown >= 0) {
				cooldown -= 6 * deltaTime;
			}
			else {
				cooldown = 2;
				for (int i = 0; i < 10; i++) {
					if (m_pLaser[i] == NULL) {
						m_pLaser[i] = new Laserbullet();
						m_pLaser[i]->Initialise(m_pCannon->Position(), *storage);
						break;
					}
					else if (!m_pLaser[i]->isAlive()) {
						m_pLaser[i]->Initialise(m_pCannon->Position(), *storage);
						break;
					}
				}
			}
		}
		for (int k = 0; k < m_iShowCount; ++k)
		{
			m_pInvaders[k]->Process(deltaTime);
			if (m_pInvaders[k]->isAlive()) {//check if processing living invader
				m_pInvaders[k]->Process(deltaTime);
				//go through each projectile
			}
			double distance =
				sqrt(
					pow(((m_pCannon->Position().x) - (m_pInvaders[k]->Position().x)), 2)
					+ pow(((m_pCannon->Position().y) - (m_pInvaders[k]->Position().y)), 2)
				) - m_pInvaders[k]->GetRadius() - m_pCannon->GetRadius();
			if (distance <= 0) {
				m_pCannon->Kill();
				break;
			}
		}

		for (int i = 0; i < 10; i++) {
			if (m_pLaser[i] != NULL) {
				if (m_pLaser[i]->isAlive()) {
					m_pLaser[i]->Process(deltaTime);
					for (int k = 0; k < m_iShowCount; ++k)
					{
						if (m_pInvaders[k]->isAlive()) {
							double distance =
								sqrt(
									pow(((m_pLaser[i]->Position().x) - (m_pInvaders[k]->Position().x)), 2)
									+ pow(((m_pLaser[i]->Position().y) - (m_pInvaders[k]->Position().y)), 2)
								) - m_pInvaders[k]->GetRadius() - m_pLaser[i]->GetRadius();
							if (distance <= 0) {
								m_pInvaders[k]->Speedup();
								m_pInvaders[k]->Kill();
								m_pLaser[i]->Kill();
								
								break;
							}
						}
					}
				}
			}

		}
		if (m_pCannon->isAlive()) {
			m_pCannon->Process(deltaTime, inputSystem);
		}
	}
	//Vector2 Positions = inputSystem.GetMousePosition();
}



void
SceneSpaceInvadersClone::Draw(Renderer& renderer)
{
	for (int k = 0; k < m_iShowCount; ++k)
	{
		m_pInvaders[k]->Draw(renderer);
	}
	for (int L = 0; L < 10; ++L)
	{
		if (m_pLaser[L] != NULL) {
			m_pLaser[L]->Draw(renderer);
		}
	}
	if (m_pCannon->isAlive()) {
		m_pCannon->Draw(renderer);
	}
}

void SceneSpaceInvadersClone::DebugDraw
()
{
	ImGui::Text("Scene: Space Invaders");
	ImGui::InputFloat2("Position", &(m_pInvaders[0]->Position().x));
}