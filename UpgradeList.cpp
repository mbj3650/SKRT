#include "UpgradeList.h"
#include "renderer.h"
#include "sprite.h"
#include "inputsystem.h"
#include "vector2.h"
#include "PlayerObject.h"
#include "inlinehelpers.h"
#include "lib/imgui/imgui.h"
#include <iostream>
UpgradeList::UpgradeList()
{
	
}

void UpgradeList::Initialize(Renderer& renderer, PlayerObject* player) {
	this->player = player;
	SCREEN_WIDTH = renderer.GetWidth();
	SCREEN_HEIGHT = renderer.GetHeight();

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

	m_upgrades.push_back({ 211,"Harder Punches",1,"attack_1","Increases player damage by 10%",212,"2,010" });
	m_upgrades.push_back({ 212,"Horseshoe Gloves",2,"attack_2","Increases player damage by 20%",213,"2,020" });
	m_upgrades.push_back({ 213,"Diamond Gauntlets",3,"attack_3","Increases player damage by 45%",0,"2,045" });

	m_upgrades.push_back({ 321,"Slingshot",1,"rebound_1","Decreases player speed loss on collision by 10%",322,"3,010" });
	m_upgrades.push_back({ 322,"Whip-back",2,"rebound_2","Decreases player speed loss on collision by 20%",323,"3,020" });
	m_upgrades.push_back({ 323,"Boomerang",3,"rebound_3","Decreases player speed loss on collision by 45%",-10,"3,045" });

	m_upgrades.push_back({ -10,"Ricochet",4,"ricochet","Upon hitting an enemy, automatically bounce towards another enemy",0,"-1" });

	SCREEN_WIDTH = renderer.GetWidth();
	SCREEN_HEIGHT = renderer.GetHeight();

	//set up all sprites, and the applicable starting list (contains all tier 1s at the start)
	for (int i = 0; i < m_upgrades.size(); i++) {
		std::string spritepath = "..\\assets\\upgrades\\";//get path of upgrade icon
		spritepath.append(m_upgrades.at(i).iconpath);
		spritepath.append(".png");
		m_upgrades.at(i).SpritePointer = i;
		Sprite* newsprite = renderer.CreateSprite(spritepath.c_str());
		spritelist[i] = newsprite;//create the list
		spritelist[i]->SetScale(2.0f);
		if (m_upgrades.at(i).tier == 1) {
			applicableupgrades.push_back(m_upgrades.at(i));
		}
	}
}

UpgradeList::~UpgradeList()
{
}

void UpgradeList::PickThree() {
	selection.clear();//clear selections
	int selectionsize = 3;//pick 3
	if (applicableupgrades.size() < 3) {//unless theres less than 3 applicable, then just pick from 
		selectionsize = applicableupgrades.size();//the size of the applicable array
	}


	for (int i = 0; i < selectionsize; i++) {//for selection 1-3 upgrades
		int toselect;//select a random upgrade from applicable upgrades
		bool found; //this will tell us how many times the picked element appears in the selection array, if its more than 0, that means it already exists and we need to reroll
		do {
			
			toselect = GetRandom(0, applicableupgrades.size()-1);//pick a random applicable one
			found = false;//assume theres none at the start
			for (int i = 0; i < selection.size(); i++) {
				if (selection.at(i).ID == applicableupgrades.at(toselect).ID) {//if id is already found inside the array, then its already inluded
					found = true;//if its already picked, reroll
				}
			}
		} while (found != false);//do this until it doesnt appear 

		selection.push_back(applicableupgrades.at(toselect));//add unique upgrade to the selection
	}//do this 3 times for 3 upgrades in selection vector


	//set the position here, since we wont need it to be constantly moving
	spritelist[selection.at(0).SpritePointer]->SetX(SCREEN_WIDTH / 2);//middle
	spritelist[selection.at(0).SpritePointer]->SetY(SCREEN_HEIGHT / 2);
	spritestodraw[0] = spritelist[selection.at(0).SpritePointer];

	if (selection.size() > 1) {
		spritelist[selection.at(1).SpritePointer]->SetX(SCREEN_WIDTH / 2 - (spritelist[selection.at(1).SpritePointer]->GetWidth() * 3));//left
		spritelist[selection.at(1).SpritePointer]->SetY(SCREEN_HEIGHT / 2);
		spritestodraw[1] = spritelist[selection.at(1).SpritePointer];
	}


	if (selection.size() > 2 ) {
		spritelist[selection.at(2).SpritePointer]->SetX(SCREEN_WIDTH / 2 + (spritelist[selection.at(1).SpritePointer]->GetWidth() * 3));//right
		spritelist[selection.at(2).SpritePointer]->SetY(SCREEN_HEIGHT / 2);
		spritestodraw[2] = spritelist[selection.at(2).SpritePointer];
	}


	
	
	showingupgrades = true;
}

void
UpgradeList::Process(float deltaTime, InputSystem& inputSystem)
{
	int result = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);
	mouse_position.x = inputSystem.GetMousePosition().x;
	mouse_position.y = inputSystem.GetMousePosition().y;
	for (int i = 0; i < selection.size(); i++) {
		if (mouse_position.x < (spritelist[selection.at(i).SpritePointer]->GetX() + spritelist[selection.at(i).SpritePointer]->GetWidth()) //IF WITHIN THE BOUNDS OF THE UPGRADE i SQUARE
			&&
			mouse_position.x >(spritelist[selection.at(i).SpritePointer]->GetX() - spritelist[selection.at(i).SpritePointer]->GetWidth())
			&&
			mouse_position.y <(spritelist[selection.at(i).SpritePointer]->GetY() + spritelist[selection.at(i).SpritePointer]->GetWidth())
			&&
			mouse_position.y >(spritelist[selection.at(i).SpritePointer]->GetY() - spritelist[selection.at(i).SpritePointer]->GetWidth())
			) {
			selected = i;//set selected
			break;
		}
		else {
			selected = -1;
		}
	}
	
	if (result == BS_PRESSED) {
		switch (selected) {
		case 0:
		
			player->AddUpgrade(selection.at(0));
			
			showingupgrades = false;
			ReplaceApplicableupgrade(selection.at(0));
			selection.clear();//clear selections
			break;
		case 1:
			
			player->AddUpgrade(selection.at(1));
			showingupgrades = false;
			ReplaceApplicableupgrade(selection.at(1));
			selection.clear();//clear selections
			
			
			break;
		case 2:
			
			player->AddUpgrade(selection.at(2));
			
			showingupgrades = false;
			ReplaceApplicableupgrade(selection.at(2));
			selection.clear();//clear selections
			break;
		default:
			break;
		}
				
	}
	
};
void UpgradeList::Draw(Renderer& renderer)
{
	for (int f = 0; f < selection.size();f++) {
		spritestodraw[f]->Draw(renderer);
	}
}
void UpgradeList::ReplaceApplicableupgrade(Template Upgradetograb)
{
	if (Upgradetograb.upgradesinto == 0) {//if upgrade cant evolve
		for (int b = 0; b < applicableupgrades.size(); b++) {//go through the applicable upgrades list
			if (applicableupgrades.at(b).ID == Upgradetograb.ID) {//until we find the upgrade we want to replace
				applicableupgrades.erase(applicableupgrades.begin() + b);//and remove it since we dont want it to appear again
			}
		}
	}
	else if (Upgradetograb.upgradesinto > 0) {//otherwise, if it has an evolution
		for (int i = 0; i < m_upgrades.size(); i++) {//go through all available upgrades
			if (Upgradetograb.upgradesinto == m_upgrades.at(i).ID) {//if upgradeinto isnt 0 and we can find a matching "evolution" in the upgrades list
				for (int b = 0; b < applicableupgrades.size(); b++) {//go through the applicable upgrades list
					if (applicableupgrades.at(b).ID == Upgradetograb.ID) {//until we find the upgrade we want to replace
						applicableupgrades.erase(applicableupgrades.begin() + b);
						applicableupgrades.push_back( m_upgrades.at(i));//and replace the original upgrade with the evolved upgrade entry
					}
				}
			}
		}
	}
	else if (Upgradetograb.upgradesinto < 0) {//if upgrade is special
		SpecialIDs.push_back(Upgradetograb.upgradesinto);//push special ID into the array
		for (int i = 0; i < SpecialIDs.size(); i++) {//go through all Special IDS 
			std::cout << "enough! \n"<< SpecialIDs.size() << "\n";
			//and if special ids appear more than once, add it to the applicable array
			if (std::count(SpecialIDs.begin(), SpecialIDs.end(), SpecialIDs.at(i)) > 1) {
				if (std::count(applicableupgrades.begin(), applicableupgrades.end(), FetchUpgrade(SpecialIDs.at(i))) < 1) {//if applicable upgrades doesnt already have the upgrade
					applicableupgrades.push_back(FetchUpgrade(SpecialIDs.at(i)));//add it
				}
			}
		}
	

		for (int b = 0; b < applicableupgrades.size(); b++) {//go through the applicable upgrades list
			if (applicableupgrades.at(b).ID == Upgradetograb.ID) {//until we find the upgrade we want to replace
				applicableupgrades.erase(applicableupgrades.begin() + b);//and remove it since we dont want it to appear again
			}
		}
	}
}
;

UpgradeList::Template UpgradeList::FetchUpgrade(int ID) {//search through array
	for (int i = 0; i < m_upgrades.size(); i++) {
		if (m_upgrades.at(i).ID == ID) {//if id matches
			return m_upgrades.at(i);//return the upgrade that matches this new upgrade
		}
	}
}


void UpgradeList::DebugDraw() {
	ImGui::Text("HOVER INFORMATION: %d", selected);
	for (int i = 0; i < applicableupgrades.size(); i++) {
		ImGui::Text("Name info: %s", applicableupgrades.at(i).name.c_str());
	}
	for (int i = 0; i < selection.size(); i++) {
		ImGui::Text("Sprite %d info: ", i);
		ImGui::Text("Name info: %s", selection.at(i).name.c_str());
		ImGui::Text("Position info: %f, %f", spritelist[selection.at(i).SpritePointer]->GetX(), spritelist[selection.at(i).SpritePointer]->GetY() );
		ImGui::Text("Address info: %s", &spritelist[selection.at(i).SpritePointer]);
	}
}
