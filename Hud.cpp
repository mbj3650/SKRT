// COMP710 JESSE
// This include:
#include "Hud.h"

// Local includes:
#include "renderer.h"
#include "PlayerObject.h"
#include "sprite.h"
// Library includes:
#include <cassert>
#include <string>
using namespace std;
Hud::Hud()
{

};
Hud::~Hud()
{
	delete HealthText;
	delete m_pSpriteHealth;
	m_pSpriteHealth, HealthText = 0;
	m_pPlayer = 0;
	storage = 0;
};


bool Hud::Initialise(Renderer& renderer, PlayerObject* player)
{
	m_pPlayer = player;
	for (int i = 0; i <= 100; i++) {//print numbers from 0 to 100;
		string number = to_string(i);
		renderer.CreateStaticText(number.c_str(), 32);
	}
	renderer.CreateStaticText("Health:", 32);
	
	storage = &renderer;
	HealthText = storage->CreateSprite("Health:");
	updatestats();
	gameover = " ";
	return false;
}

void Hud::updatestats() {
	HealthText->SetY(HealthText->GetHeight() / 2);
	HealthText->SetX(HealthText->GetWidth()/2);
	if (m_pSpriteHealth == NULL) {//if sprite isnt null
		m_pSpriteHealth = storage->CreateSprite(to_string(m_pPlayer->GetHealth()).c_str());
		m_pSpriteHealth->SetY(m_pSpriteHealth->GetHeight()/2);
		m_pSpriteHealth->SetX(m_pSpriteHealth->GetWidth()/2 + HealthText->GetWidth());
	}
}

void Hud::DisplayScore() {
	if (gameover.size() < 3) {
		gameover = "YOU DIED!  SCORE: ";
		gameover.append(to_string(m_pPlayer->GetScore()));
		storage->CreateStaticText(gameover.c_str(), 32);
		restart = "PRESS R TO RESTART";
		storage->CreateStaticText(restart.c_str(), 32);
		text = storage->CreateSprite(gameover.c_str());
		text2 = storage->CreateSprite(restart.c_str());
	}
}

void
Hud::Process(float deltaTime)
{
	updatestats();
};

void Hud::GameWipe() {
	delete text;
	delete text2; 
	text2, text = 0;
	gameover = " ";
}

void Hud::SetPlayer(PlayerObject* player) {
	m_pPlayer = player;
}


void
Hud::Draw(Renderer& renderer)
{
	if (m_pPlayer->isAlive()) {
		HealthText->Draw(renderer);
		m_pSpriteHealth->Draw(renderer);
		delete m_pSpriteHealth;
		m_pSpriteHealth = 0;
	}
	else {
		DisplayScore();
		text2->SetX(storage->GetWidth() / 2);
		text->SetX(storage->GetWidth()/2);
		text2->SetY(storage->GetHeight() / 2 + text2->GetHeight());
		text->SetY(storage->GetHeight() / 2);
		text2->Draw(renderer);
		text->Draw(renderer);
	}
	
};
