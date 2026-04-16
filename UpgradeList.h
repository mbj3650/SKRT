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
		UpgradeList();
		void Initialize(Renderer& renderer, PlayerObject* player);
		~UpgradeList();
		void PickThree();
		void Process(float deltaTime, InputSystem& inputSystem);
		void Draw(Renderer& renderer);
		void DebugDraw();
	protected:

	private:
		UpgradeList(const UpgradeList& UpgradeList);
		UpgradeList& operator=(const UpgradeList& UpgradeList);

		// Member data:
	public:
		class Template {
		public:
			int ID;
			std::string name;
			int tier;
			std::string iconpath;
			std::string description;
			int upgradesinto;
			std::string stateffect;
			int SpritePointer;//points to element index in array of sprites in the upgrade list class
		};
		bool showingupgrades;
	protected:
		int selected;
		PlayerObject* player;
		Vector2 mouse_position;
		Vector2 offsetposition;
		int timer;
		std::vector<Template> m_upgrades;
		std::vector<Template> applicableupgrades;
		std::vector<Template> selection;
	
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
		Sprite* spritelist[100];
		Sprite* spritestodraw[3];
	private:

};
