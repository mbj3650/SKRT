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
	selected = -1;

	//SET UP SPRITES
	Menu = renderer.CreateSprite("..\\assets\\upgrades\\upgrademenu.png");
	Menu->SetScale(0.5f);
	Menu->SetX(SCREEN_WIDTH / 2);
	Menu->SetY(SCREEN_HEIGHT / 2);


	Skip = renderer.CreateSprite("..\\assets\\upgrades\\skipbutton.png");
	Skip->SetScale(0.5f);

	SkipSpecial = renderer.CreateSprite("..\\assets\\upgrades\\skipbutton_special.png");
	SkipSpecial->SetScale(0.5f);


	//set up all sprites, and the applicable starting list (contains all tier 1s at the start)
	for (int i = 0; i < m_upgrades.size(); i++) {
		std::string spritepath = "..\\assets\\upgrades\\";//get path of upgrade icon
		spritepath.append(m_upgrades.at(i).iconpath);
		spritepath.append(".png");
		m_upgrades.at(i).SpritePointer = i;
		Sprite* newsprite = renderer.CreateSprite(spritepath.c_str());
		spritelist[i] = newsprite;//create the list
		spritelist[i]->SetScale((64 / newsprite->GetWidth()));
		if (m_upgrades.at(i).tier == 1) {
			applicableupgrades.push_back(m_upgrades.at(i));
		}

	}
	for (int i = 0; i < m_upgrades.size(); i++) {

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
	if (specialapplicableupgrades.size() > 0 && skipped == false) {
		anticipationskip = true;
	}

	bool selectedspecial = false;
	for (int i = 0; i < selectionsize; i++) {//for selection 1-3 upgrades
		int toselect;//select a random upgrade from applicable upgrades
		bool found; //this will tell us how many times the picked element appears in the selection array, if its more than 0, that means it already exists and we need to reroll
		if (specialapplicableupgrades.size() > 0 && skipped == true && selectedspecial == false) {//if player skipped and theres an applicable special upgrade, guarantee one
			toselect = GetRandom(0, specialapplicableupgrades.size() - 1);//pick a random applicable one
			selection.push_back(specialapplicableupgrades.at(toselect));//add it 
			anticipationskip = false;
			selectedspecial = true;
		}
		else {
			do {
				toselect = GetRandom(0, applicableupgrades.size() - 1);//pick a random applicable one
				found = false;//assume theres none at the start
				for (int i = 0; i < selection.size(); i++) {
					if (selection.at(i).ID == applicableupgrades.at(toselect).ID) {//if id is already found inside the array, then its already inluded
						found = true;//if its already picked, reroll
					}
				}
			} while (found != false);//do this until it doesnt appear 
		}
		
		selection.push_back(applicableupgrades.at(toselect));//add unique upgrade to the selection
	}//do this 3 times for 3 upgrades in selection vector

	if (selection.size() <= 0) {

	}
	else {
		//set the position here, since we wont need it to be constantly moving
		spritelist[selection.at(0).SpritePointer]->SetX(SCREEN_WIDTH / 2);//middle
		spritelist[selection.at(0).SpritePointer]->SetY(SCREEN_HEIGHT / 2);
		spritestodraw[0] = spritelist[selection.at(0).SpritePointer];

		if (selection.size() > 1) {
			spritelist[selection.at(1).SpritePointer]->SetX(SCREEN_WIDTH / 2 - (spritelist[selection.at(1).SpritePointer]->GetWidth() * 2.75));//left
			spritelist[selection.at(1).SpritePointer]->SetY(SCREEN_HEIGHT / 2);
			spritestodraw[1] = spritelist[selection.at(1).SpritePointer];
		}


		if (selection.size() > 2) {
			spritelist[selection.at(2).SpritePointer]->SetX(SCREEN_WIDTH / 2 + (spritelist[selection.at(1).SpritePointer]->GetWidth() * 2.75));//right
			spritelist[selection.at(2).SpritePointer]->SetY(SCREEN_HEIGHT / 2);
			spritestodraw[2] = spritelist[selection.at(2).SpritePointer];
		}
		offsetposition.y = -(SCREEN_HEIGHT / 2);
	}
	
	
	showingupgrades = true;
}

Sprite* UpgradeList::CreateText(char string[]) {
	return NULL;
}


void
UpgradeList::Process(float deltaTime, InputSystem& inputSystem)
{

	if (showingupgrades = true) {
		if (offsetposition.y < 0) {
			offsetposition.y += deltaTime * SCREEN_HEIGHT * (offsetposition.y/-100);
			Menu->SetY(offsetposition.y + (SCREEN_HEIGHT / 2));
			for (int i = 0; i < selection.size(); i++) {
				spritestodraw[i]->SetY(offsetposition.y + (SCREEN_HEIGHT / 2));
			}
		}



	}

	int result = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);
	mouse_position.x = inputSystem.GetMousePosition().x;
	mouse_position.y = inputSystem.GetMousePosition().y;
	if (selection.size() > 0) {//if theres upgrades to choose from check for buttons
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
			else if (mouse_position.x < Skip->GetX() + Skip->GetWidth() //IF WITHIN THE BOUNDS OF THE UPGRADE i SQUARE
				&&
				mouse_position.x >(Skip->GetX() - Skip->GetWidth())
				&&
				mouse_position.y <(Skip->GetY() + Skip->GetHeight())
				&&
				mouse_position.y >(Skip->GetY() - Skip->GetHeight())
				) {
				selected = 99;//set selected
				break;
			}
			else {
				selected = -1;
			}
		}
	}
	else {//else just check if user pressed skip
		if (mouse_position.x < Skip->GetX() + Skip->GetWidth() //IF WITHIN THE BOUNDS OF THE UPGRADE i SQUARE
			&&
			mouse_position.x >(Skip->GetX() - Skip->GetWidth())
			&&
			mouse_position.y <(Skip->GetY() + Skip->GetHeight())
			&&
			mouse_position.y >(Skip->GetY() - Skip->GetHeight())
			) {
			selected = 99;//set selected
		}
		else {
			selected = -1;
		}
	}
	


	
	if (result == BS_PRESSED) {
		if (selected != -1) {
			if (selected != 99) {//if player didnt skip
				player->AddUpgrade(selection.at(selected));
				showingupgrades = false;
				skipped = false;//they didnt skip
				ReplaceApplicableupgrade(selection.at(selected));
				selection.clear();//clear selections
			}
			else {
				player->AddUpgrade(0);
				showingupgrades = false;
				skipped = true;//they can now get a special upgrade
				selection.clear();//clear selections
			}
		}				
	}

	SkipSpecial->SetX(Menu->GetX() + Menu->GetWidth() / 2);
	SkipSpecial->SetY(Menu->GetY() - Menu->GetHeight() / 2);

	Skip->SetX(Menu->GetX() + Menu->GetWidth() / 2);
	Skip->SetY(Menu->GetY() - Menu->GetHeight()/2);
};
void UpgradeList::Draw(Renderer& renderer)
{
	if (pausemenuupgrades) {
		int yoffset = 0;
		for (int i = 0; i < player->GetUgprades().size(); i++) {//rows of 5 upgrades each row
			Sprite* currentsprite = spritelist[player->GetUgprades().at(i).SpritePointer];
			currentsprite->SetX(32 + (currentsprite->GetWidth() * (i%5)));
			currentsprite->SetY(32 + (currentsprite->GetHeight() * (i / 5)));
			currentsprite->Draw(renderer);
		}
	}
	if (showingupgrades) {//if showing upgrades
		Menu->Draw(renderer);
		if (selection.size() > 0) {
			if (selected >= 0 && selected < 10) {
				renderer.CreateStaticText(selection.at(selected).name.c_str(), 16);
				renderer.CreateStaticText(selection.at(selected).description.c_str(), 16);
				NameHover = renderer.CreateSprite(selection.at(selected).name.c_str());
				DescriptionHover = renderer.CreateSprite(selection.at(selected).description.c_str());
				NameHover->SetX(Menu->GetX());
				NameHover->SetY(Menu->GetY() - 100);

				DescriptionHover->SetX(Menu->GetX());
				DescriptionHover->SetY(Menu->GetY() + 100);


				NameHover->Draw(renderer);
				DescriptionHover->Draw(renderer);

			}
		}
		if (anticipationskip == true) {
			SkipSpecial->Draw(renderer);
		}
		else {
			Skip->Draw(renderer);
		}

		for (int f = 0; f < selection.size(); f++) {
			spritestodraw[f]->Draw(renderer);
		}
	}


}

void UpgradeList::ReplaceApplicableupgrade(Template Upgradetograb)
{
	if (Upgradetograb.upgradesinto == 0) {//if upgrade cant evolve
		if (Upgradetograb.ID > 0) {//if its not special
			for (int b = 0; b < applicableupgrades.size(); b++) {//go through the applicable upgrades list
				if (applicableupgrades.at(b).ID == Upgradetograb.ID) {//until we find the upgrade we want to replace
					applicableupgrades.erase(applicableupgrades.begin() + b);//and remove it since we dont want it to appear again
				}
			}
		}
		else {
			for (int b = 0; b < specialapplicableupgrades.size(); b++) {//go through the applicable upgrades list
				if (specialapplicableupgrades.at(b).ID == Upgradetograb.ID) {//until we find the upgrade we want to replace
					specialapplicableupgrades.erase(specialapplicableupgrades.begin() + b);//and remove it since we dont want it to appear again
				}
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
			//and if special ids appear more than once, add it to the applicable array
			if (std::count(SpecialIDs.begin(), SpecialIDs.end(), SpecialIDs.at(i)) > 1) {
				if (std::count(specialapplicableupgrades.begin(), specialapplicableupgrades.end(), FetchUpgrade(SpecialIDs.at(i))) < 1) {//if applicable upgrades doesnt already have the upgrade
					specialapplicableupgrades.push_back(FetchUpgrade(SpecialIDs.at(i)));//add it to applicable special upgrades
					anticipationskip = true;
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
	for (int i = 0; i < specialapplicableupgrades.size(); i++) {
		ImGui::Text("Name info: %s", specialapplicableupgrades.at(i).name.c_str());
	}
	for (int i = 0; i < selection.size(); i++) {
		ImGui::Text("Sprite %d info: ", i);
		ImGui::Text("Name info: %s", selection.at(i).name.c_str());
		ImGui::Text("Position info: %f, %f", spritelist[selection.at(i).SpritePointer]->GetX(), spritelist[selection.at(i).SpritePointer]->GetY() );
		ImGui::Text("Address info: %s", &spritelist[selection.at(i).SpritePointer]);
	}
}
