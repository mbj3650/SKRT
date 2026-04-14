#pragma once
#include <string>
#include <vector>
class UpgradeList
{
	public:
		UpgradeList();
		~UpgradeList();
		struct Template {
			int ID;
			std::string name;
			int tier;
			std::string iconpath;
			std::string description;
			int upgradesinto;
			
		};
	protected:

	private:
		UpgradeList(const UpgradeList& UpgradeList);
		UpgradeList& operator=(const UpgradeList& UpgradeList);

		// Member data:
	public:
		std::vector<Template> m_upgrades;
	protected:
	
	private:

};
