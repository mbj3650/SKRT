// COMP710 Steffan Hooper
// This include:
#include "SceneBallGame.h"
// Local includes:
#include "renderer.h"
#include "entityball.h"
#include "ball.h"
// Library includes:
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "lib/imgui/imgui.h"
SceneBallGame::SceneBallGame()
	: m_pBalls{ 0 }
	, m_iShowCount(0)
	, Goodball(0),
	Badball(0)
{
}
SceneBallGame::~SceneBallGame()
{
	for (int k = 0; k < 100; ++k)
	{
		delete m_pBalls[k];
		m_pBalls[k] = 0;
	}
}
bool
SceneBallGame::Initialise(Renderer& renderer)
{
	storage = &renderer;
	m_iShowCount = 0;
	makegoodball(10, renderer);
	makebadball(10, renderer);
	m_pBalls[0] = new EntityBall();
	m_pBalls[0]->Initialise(renderer);
	// Always place one ball at the centre...
	m_pBalls[0]->Position().x = renderer.GetWidth() / 2.0f;
	m_pBalls[0]->Position().y = renderer.GetHeight() / 2.0f;
	m_pBalls[0]->SetNeutral();
	m_iShowCount++;
	return true;
}

void SceneBallGame::CheckNeutralCollisions() {
	for (int k = 1; k < 100; k++)
	{
		if (m_pBalls[k] != NULL) {
			
			double distance =
				sqrt(
					pow(((m_pBalls[0]->Position().x) - (m_pBalls[k]->Position().x)), 2)
					+ pow(((m_pBalls[0]->Position().y) - (m_pBalls[k]->Position().y)), 2)
				) - m_pBalls[k]->GetRadius() - m_pBalls[0]->GetRadius();
			if (distance < 10) {
				std::cout << distance << "\n";
			}
			
			if (distance <= 0) {
				
				if (m_pBalls[k]->GetGB() == 1) {
					m_pBalls[0]->UpdateScale(1.05);
				}
				else if (m_pBalls[k]->GetGB() == 2) {
					m_pBalls[0]->UpdateScale(0.95);
				}
				delete m_pBalls[k];
				m_pBalls[k] = 0;
			}
		}
	}
}

void
SceneBallGame::Process(float deltaTime, InputSystem& inputSystem)
{
	CheckNeutralCollisions();
	for (int k = 0; k < 100; ++k)
	{
		if (m_pBalls[k] != NULL) {
			m_pBalls[k]->Process(deltaTime);
		}
	}
	int posX;
	int posY;
	Vector2 Positions = inputSystem.GetMousePosition();
	m_pBalls[0]->Position().x = Positions.x;
	m_pBalls[0]->Position().y = Positions.y;
}

void SceneBallGame::makegoodball(int ballstomake, Renderer& renderer) {
	int k = 1;
	while (k != 99 && ballstomake > 0) {
		if (m_pBalls[k] == 0) {
			m_pBalls[k] = new EntityBall();
			m_pBalls[k]->Initialise(renderer);
			m_pBalls[k]->SetGood();
			m_pBalls[k]->Position().y = m_pBalls[k]->Position().y / 2;
			m_iShowCount++;
			Goodball++;
			ballstomake--;
		}
		k++;
	}
}

void SceneBallGame::makebadball(int ballstomake, Renderer& renderer) {
	int k = 1;
	while (k != 99 && ballstomake > 0) {
		if (m_pBalls[k] == 0) {
			m_pBalls[k] = new EntityBall();
			m_pBalls[k]->Initialise(renderer);
			m_pBalls[k]->SetBad();
			const int SCREEN_HEIGHT = renderer.GetHeight();
			m_pBalls[k]->Position().y = SCREEN_HEIGHT-(m_pBalls[k]->Position().y / 2);
			m_iShowCount++;
			Badball++;
			ballstomake--;
		}
		k++;
	}
}
void
SceneBallGame::Draw(Renderer& renderer)
{
	for (int k = 0; k < 100; ++k)
	{
		if (m_pBalls[k] != NULL) {
			m_pBalls[k]->Draw(renderer);
		}
	}
}

void SceneBallGame::DebugDraw
()
{
	ImGui::Text("Scene: Balls Game");
	//ImGui::SliderInt("Show Count", &m_iShowCount, 1, Goodball+Badball+1);
	if (ImGui::Button("Spawn GOOD ball"))
	{
		makegoodball(1, *storage);
	}
	if (ImGui::Button("Spawn GOOD ballx10"))
	{
		makegoodball(10, *storage);
	}
	if (ImGui::Button("Spawn BAD ball"))
	{
		makebadball(1, *storage);
	}
	if (ImGui::Button("Spawn BAD ballx10"))
	{
		makebadball(10, *storage);
	}
}