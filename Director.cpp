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
#include "Tree.h"
#include "Bigboss.h"
#include <vector>
#include "IniParser.h"
#include "lib/imgui/imgui.h"
#include "Slower.h"
#include "inlinehelpers.h"
Director::Director():
EnemyPriceMultiplier { 0,0,0,0 },
Timetospawn(1),
Timepassed(0)
{
	
};
Director::~Director()
{
	for (int i = 0; i < m_pDirectorArray->size(); i++) {
		delete m_pDirectorArray->at(i);
		m_pDirectorArray->at(i) = 0;
	}
	m_pDirectorArray->clear();
};

bool Director::Initialise(Renderer& renderer, std::vector<EnemyBase*> &EnemyArray, PlayerObject* m_pPlayerChar ,b2WorldId WorldP)
{
	Credits = 0;
	Spawnboss = true;//if true, director can spawn boss
	Arraysize = sizeof(EnemyPrice) / sizeof(EnemyPrice[0]);
	storage = &renderer;
	m_pPlayer = m_pPlayerChar;
	Worldref = WorldP;
	m_pDirectorArray = &EnemyArray;
	IniParser Parser;
	Parser.LoadIniFile("..\\assets\\ini\\enemyprices.ini");
	EnemyPrice[DEMON] = Parser.GetValueAsInt("DEMON");
	EnemyPrice[SLOWER] = Parser.GetValueAsInt("SLOWER");
	EnemyPrice[MINELAYER] = Parser.GetValueAsInt("MINELAYER");
	EnemyPrice[BOSS] = Parser.GetValueAsInt("BIGBOSS");
	//we get the area and then divide by the tree value to get how many trees to spawn
	totaltrees = Parser.GetValueAsInt("TREES");
	std::cout << totaltrees << "\n";
	spawnedtrees = false;
	return true;
};

void Director::SpawnTrees() {
	spawnedtrees = true;
	for (int i = 0; i < totaltrees; i++) {
		m_pDirectorArray->push_back(new Tree());
		m_pDirectorArray->at(m_pDirectorArray->size() - 1)->Initialise(*storage, m_pPlayer->ID, Worldref);
	}
}

void
Director::Process(float deltaTime)
{
	if (bosstimer > 0) {//after a boss is spawned, set th etimer to 60 seconds and when it reaches 0 allow the boss to be spawned again
		bosstimer -= deltaTime;
	}
	else if(Spawnboss == false){
		Spawnboss = true;
	}
	if (spawnedtrees == false) {
		SpawnTrees();
	}
	if (Credits < 150) {
		float timepasscreditbonus = deltaTime * (Timepassed / 100);// slowly gain more credits overtime
		Credits += timepasscreditbonus + deltaTime;
	}
	for (int i = 0; i < Arraysize; i++) {//for every enemy
		//increase price overtime, this works in a way that common enemies become rarer overtime, and rarer ones more common
		EnemyPriceMultiplier[i] += deltaTime * (Timepassed / 100 / EnemyPrice[i]);
	}

	for (int z = 0; z < Arraysize; z++) {
		if (EnemyPriceMultiplier[z] > 0) {
			EnemyPriceMultiplier[z] -= deltaTime/10;
			if (EnemyPriceMultiplier[z] < 0) {
				EnemyPriceMultiplier[z] = 0;
			}
		}
	}


	if (Timetospawn <= 0) {//if time between enemies is not on cooldown

		
		Timetospawn = GetRandom(0, 2);//pick a random time to spawn next enemy
		Enemies current;
		if (Spawnboss == true) {//if can spawn boss
			current = BOSS;//pick Boss
		}
		else {
			current = SLOWER;//pick end enemy		
		}
		for (int i = 0; i < Arraysize; i++) {//go trhough all possible enemies
			int totalcost = EnemyPrice[current] * (1 + (EnemyPriceMultiplier[current]));//to stop the game from spamming the same enemy, make it more expensive until the game spawns something else
			if (totalcost > Credits) {//if enemyprice is too expensive
				current = static_cast<Enemies>(current - 1);//go down an enemy
				if (current == -1) {//if cant spawn enemy
					break;//skip
				}
			}
			else {//else spawn the enemy
				CreateEnemy(current);
				if (EnemyPrice[current]* (EnemyPriceMultiplier[current] + 1) < 30) {//we dont want the multiplier to reach a point that it cant spawn a certain enemy, so we cap it out the big boost at 30
					EnemyPriceMultiplier[current] += 1;
				}
			
				Credits -= totalcost;
				for (int g = 0; g < Arraysize; g++) {
					if (g != current) {//if g doesnt equal chosen enemy
						EnemyPriceMultiplier[g] -= ((1 / EnemyPrice[g]) * 5);
						if (EnemyPriceMultiplier[g] < 0) {
							EnemyPriceMultiplier[g] = 0;
						}
					}
				}
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
			case BOSS:
				m_pDirectorArray->push_back(new Bigboss());
				Spawnboss = false;
				bosstimer = 60;
				break;
			case SLOWER:
				m_pDirectorArray->push_back(new Slower());
				break;
			case MINELAYER:
				m_pDirectorArray->push_back(new Minelayer());
				break;
			case DEMON:
				m_pDirectorArray->push_back(new EnemyBase());
				break;
			}
			TotalEntities++;
			m_pDirectorArray->at(m_pDirectorArray->size()-1)->Initialise(*storage, m_pPlayer->ID, Worldref, {-9999,-9999});
}

void Director::CreateFriends() {
	m_pDirectorArray->push_back(new Minelayer());
	m_pDirectorArray->at(m_pDirectorArray->size() - 1)->Initialise(*storage, m_pPlayer->ID, Worldref, { -9999,-9999 });
	m_pDirectorArray->push_back(new Slower());
	m_pDirectorArray->at(m_pDirectorArray->size() - 1)->Initialise(*storage, m_pPlayer->ID, Worldref, { -9999,-9999 });
	m_pDirectorArray->push_back(new Slower());
	m_pDirectorArray->at(m_pDirectorArray->size() - 1)->Initialise(*storage, m_pPlayer->ID, Worldref, { -9999,-9999 });
	m_pDirectorArray->push_back(new EnemyBase());
	m_pDirectorArray->at(m_pDirectorArray->size() - 1)->Initialise(*storage, m_pPlayer->ID, Worldref, { -9999,-9999 });
	m_pDirectorArray->push_back(new EnemyBase());
	m_pDirectorArray->at(m_pDirectorArray->size() - 1)->Initialise(*storage, m_pPlayer->ID, Worldref, { -9999,-9999 });
	TotalEntities+= 5;
}


void
Director::DebugDraw()
{
	ImGui::Text("DIRECTOR INFORMATION:");
	ImGui::Text("Credits: %f", Credits);
	ImGui::Text("BOSS TIMER: %f", bosstimer);
	ImGui::Text("Demon Price: %f",  EnemyPriceMultiplier[0]);
	ImGui::Text("Demon2 Price: %f",   EnemyPriceMultiplier[1]);
	ImGui::Text("Demon3 Price: %f",  EnemyPriceMultiplier[2]);
	ImGui::Text("Demon4 Price: %f",  EnemyPriceMultiplier[3]);
};