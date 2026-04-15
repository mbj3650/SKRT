#pragma once
#include <string>
#include <vector>
class Sprite;
class Renderer;
class UpgradeList
{
	public:
		UpgradeList();
		void Initialize(Renderer& renderer);
		~UpgradeList();
		struct Template {
			int ID;
			std::string name;
			int tier;
			std::string iconpath;
			std::string description;
			int upgradesinto;
			std::string stateffect;
			Sprite* Spriteobject;
		};
	protected:

	private:
		UpgradeList(const UpgradeList& UpgradeList);
		UpgradeList& operator=(const UpgradeList& UpgradeList);

		// Member data:
	public:
		std::vector<Template> m_upgrades;
	protected:
		Sprite* spritelist[100];
	private:

};
