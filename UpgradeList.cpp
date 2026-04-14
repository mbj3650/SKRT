#include "UpgradeList.h"

UpgradeList::UpgradeList()
{
	//int ID;
	//std::string name;
	//int tier;
	//int effect;
	//std::string iconpath;
	//std::string description;
	//int upgradesinto;
	//Name, cost, effect, icon, description
	m_upgrades.push_back({ 101,"Swift Shift",1,"speed_1","Increases player speed by 10%",102 });
	m_upgrades.push_back({ 102,"Quickened Reflexes",2,"speed_2","Increases player speed by 20%",103 });
	m_upgrades.push_back({ 103,"Need for Speed",3,"speed_3","Increases player speed by 45%",-1 });
	m_upgrades.push_back({ 111,"Harder Punches",1,"attack_1","Increases player damage by 10%",102 });
	m_upgrades.push_back({ 112,"Horseshoe Gloves",2,"attack_2","Increases player damage by 20%",103 });
	m_upgrades.push_back({ 113,"Diamond Gauntlets",3,"attack_3","Increases player damage by 45%",-1 });
}

UpgradeList::~UpgradeList()
{
}
