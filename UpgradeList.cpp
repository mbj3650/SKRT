#include "UpgradeList.h"
#include "renderer.h"
#include "sprite.h"
UpgradeList::UpgradeList()
{
	
}

void UpgradeList::Initialize(Renderer& renderer) {
	//int ID;
	//std::string name;
	//int tier;
	//int effect;
	//std::string iconpath;
	//std::string description;
	//int upgradesinto;
	// stateffect 
	//Sprite object

	//Name, cost, effect, icon, description,stateffect

	m_upgrades.push_back({ 101,"Swift Shift",1,"speed_1","Increases player speed by 10%",102,"1,010" });
	m_upgrades.push_back({ 102,"Quickened Reflexes",2,"speed_2","Increases player speed by 20%",103,"1,020" });
	m_upgrades.push_back({ 103,"Need for Speed",3,"speed_3","Increases player speed by 45%",-10,"1,045" });//negative numbers are special upgrades, while 0 means its the end

	m_upgrades.push_back({ 111,"Harder Punches",1,"attack_1","Increases player damage by 10%",102,"2,010" });
	m_upgrades.push_back({ 112,"Horseshoe Gloves",2,"attack_2","Increases player damage by 20%",103,"2,020" });
	m_upgrades.push_back({ 113,"Diamond Gauntlets",3,"attack_3","Increases player damage by 45%",-10,"2,045" });

	m_upgrades.push_back({ -10,"Ricochet",4,"speedattack_4","Upon hitting an enemy, automatically bounce towards another enemy",0,"-1" });


	//set up all sprites
	for (int i = 0; i < m_upgrades.size(); i++) {
		std::string spritepath = "..\\assets\\upgrades\\";//get path of upgrade icon
		spritepath.append(m_upgrades.at(i).iconpath);
		spritepath.append(".png");
			m_upgrades.at(i).Spriteobject = renderer.CreateSprite(spritepath.c_str());//create the list
	}
}

UpgradeList::~UpgradeList()
{
}
