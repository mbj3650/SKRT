#pragma once
#include <string>
#include <vector>
#include "vector2.h"
class Sprite;
class Renderer;
class InputSystem;
class PlayerObject;
class Vector2;
class UpgradeList
{
	
	public:
		struct Template {
			int ID;
			std::string name;
			int tier;
			std::string iconpath;
			std::string description;
			int upgradesinto;
			std::string stateffect;
			int SpritePointer;//points to element index in array of sprites in the upgrade list class

			friend bool operator==(const Template& p1, const Template& p2) {
				return p1.ID == p2.ID;
			}
		};


		UpgradeList();
		void Initialize(Renderer& renderer, PlayerObject* player);
		~UpgradeList();
		void PickThree();
		void Process(float deltaTime, InputSystem& inputSystem);
		void Draw(Renderer& renderer);
		void ReplaceApplicableupgrade(Template Upgradetograb);
		void DebugDraw();
		UpgradeList::Template  FetchUpgrade(int ID);
	protected:

	private:
		UpgradeList(const UpgradeList& UpgradeList);
		UpgradeList& operator=(const UpgradeList& UpgradeList);

		// Member data:
	public:
		
		bool showingupgrades;
	protected:
		int selected;
		PlayerObject* player;
		Vector2 mouse_position;
		Vector2 offsetposition;
		int timer;
		std::vector<Template> m_upgrades;//all upgrades
		std::vector<Template> applicableupgrades;//upgrades the player can currently get
		std::vector<Template> selection;//3 selected upgrade choices
		std::vector<int> SpecialIDs;//holds all special ids from upgradeables
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
		Sprite* spritelist[100];
		Sprite* spritestodraw[3];
	private:

};


