#pragma once
#include <string>
#include <vector>
class UpgradeList
{
	public:
		UpgradeList();
		~UpgradeList();
		struct Template {
			int cost;
			int effect;
			std::string name;
			std::string iconpath;
			std::string description;
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
