// COMP710 JESSE
// This include:
#include "Director.h"

// Local includes:
#include "renderer.h"

// Library includes:
#include <cassert>
#include "EnemyBase.h"
#include "Minelayer.h"
#include "PlayerObject.h"
#include "box2d.h"
#include <iostream>
#include <vector>
#include "lib/imgui/imgui.h"
#include "inlinehelpers.h"
Director::Director():
EnemyPrice { 2,4,8 },
EnemyPriceMultiplier { 0,0,0 },
Timetospawn(1),
Timepassed(0)
{
	
};
Director::~Director()
{

};

bool Director::Initialise(Renderer& renderer, std::vector<EnemyBase*> &EnemyArray, PlayerObject* m_pPlayerChar ,b2WorldId WorldP)
{
;
Credits = 0;
	Arraysize = sizeof(EnemyPrice) / sizeof(EnemyPrice[0]);
	storage = &renderer;
	m_pPlayer = m_pPlayerChar;
	Worldref = WorldP;
	m_pDirectorArray = &EnemyArray;

	return true;
};

void
Director::Process(float deltaTime)
{
	if (Credits < 150) {
		float timepasscreditbonus = deltaTime * (Timepassed / 100);// slowly gain more credits overtime
		Credits += timepasscreditbonus + deltaTime;
	}
	for (int i = 0; i < Arraysize; i++) {//for every enemy
		//increase price overtime, this works in a way that common enemies become rarer overtime, and rarer ones more common
		EnemyPriceMultiplier[i] += deltaTime * (Timepassed / 100 / EnemyPrice[i]);
	}
	if (Timetospawn <= 0) {//if time between enemies is not on cooldown
		
		Timetospawn = GetRandom(0, 2);//pick a random time to spawn next enemy
		Enemies current = SLOWER;//pick end enemy		
		for (int i = 0; i < Arraysize; i++) {//go trhough all possible enemies
			int totalcost = EnemyPrice[current] + (EnemyPriceMultiplier[current]);//to stop the game from spamming the same enemy, make it more expensive until the game spawns something else
			if (totalcost > Credits) {//if enemyprice is too expensive
				current = static_cast<Enemies>(current - 1);//go down an enemy
				if (current == -1) {//if cant spawn enemy
					break;//skip
				}
			}
			else {//else spawn the enemy
				CreateEnemy(current);
				Credits -= totalcost;
			}
		}
		
	}
	else {
		Timetospawn -= deltaTime * 1;
	}
	Timepassed += deltaTime * 1;
};
void
Director::Draw(Renderer& renderer)
{
	
};

void Director::AddCredits(int exp) {
	Credits += exp;
}

void Director::CreateEnemy(Enemies input) {
			switch (input) {//spawn corressponding enemy
			case 2:
				m_pDirectorArray->push_back(new Minelayer());
				break;
			case 1:
				m_pDirectorArray->push_back(new EnemyBase());
				break;
			case 0:
				m_pDirectorArray->push_back(new EnemyBase());
				break;
			}
			TotalEntities++;
			m_pDirectorArray->at(m_pDirectorArray->size()-1)->Initialise(*storage, m_pPlayer->ID, Worldref, {-9999,-9999});
}


void
Director::DebugDraw()
{
	ImGui::Text("DIRECTOR INFORMATION:");
	ImGui::Text("Credits: %f", Credits);
	ImGui::Text("Demon Price: %f", EnemyPriceMultiplier[0]);
	ImGui::Text("Demon2 Price: %f", EnemyPriceMultiplier[1]);
	ImGui::Text("Demon3 Price: %f", EnemyPriceMultiplier[2]);
};