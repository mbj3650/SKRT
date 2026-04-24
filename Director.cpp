// COMP710 JESSE
// This include:
#include "Director.h"

// Local includes:
#include "renderer.h"

// Library includes:
#include <cassert>
#include "EnemyBase.h"
#include "PlayerObject.h"
#include "box2d.h"
#include <iostream>
#include <vector>
#include "inlinehelpers.h"
Director::Director()

{
	
};
Director::~Director()
{

};

bool Director::Initialise(Renderer& renderer, std::vector<EnemyBase*>& EnemyArray, PlayerObject* m_pPlayerChar ,b2WorldId WorldP)
{
	int EnemyPrice[] = {5,12,20};
	storage = &renderer;
	m_pPlayer = m_pPlayerChar;
	Worldref = WorldP;
	m_pDirectorArray = &EnemyArray;
	Credits = 100;
	Timetospawn = 1;
	Timepassed = 0;
	return true;
};

void
Director::Process(float deltaTime)
{
	if (Credits < 150) {
		float timepasscreditbonus = (Timepassed / 100) * (Timepassed / 100);// slowly gain more credits overtime
		Credits += deltaTime + timepasscreditbonus;
	}
	if (Timetospawn <= 0) {
		Timetospawn = GetRandom(0,2);
		Enemies current = SLOWER;
		for (int i = 0; i < sizeof(EnemyPrice); i++) {
			if (EnemyPrice[current] > Credits) {//if enemyprice is too expensive
				current = static_cast<Enemies>(current - 1);//go down an enemy
			}
			else {//else spawn the enemy
				CreateEnemy(current);
				break;
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

void Director::CreateEnemy(Enemies input) {
	for (int z = 0; z < 100; z++) {
			Credits -= EnemyPrice[input];//decrease credits
			switch (input) {//spawn corressponding enemy
			case 2:
				m_pDirectorArray->push_back(new EnemyBase());
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
			break;
	}
}
