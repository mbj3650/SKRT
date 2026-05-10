// COMP710 GP Framework 2022
// This include:
#include "IniParser.h"
// Local includes:
#include "renderer.h"
#include <map>
#include <string>
#include <fstream>
using namespace std;
// Library includes:
#include <cassert>
IniParser::IniParser()
{
}
IniParser::~IniParser()
{
}

bool IniParser::LoadIniFile(const std::string& filename)
{
	string Text;
	ifstream Reader(filename);
	while (getline(Reader, Text)) {
		// Output the text from the file
		string value1 = Text.substr(0, Text.find('|')); //split at "|"
		string value2 = Text.substr(Text.find('|')+1, Text.find('|')); //split at "|"
		std::cout << value1 << "\n"<<value2 << "\n";
		layout.insert({value1, value2});//add first value and then second  
	}
	Reader.close();
	return true;
}

std::string IniParser::GetValueAsString(const std::string& key)
{
	auto str = layout.find(key);
	if (str != layout.end()) {
		return (str->second);
	}
	else {
		return "";
	}
}

int IniParser::GetValueAsInt(const std::string& key)
{
	auto str = layout.find(key);
	if (str != layout.end()) {
		return atoi(str->second.c_str());
	}
	else {
		return -1;
	}
}

float IniParser::GetValueAsFloat(const std::string& key)
{
	auto str = layout.find(key);
	if (str != layout.end()) {
		return atof(str->second.c_str());
	}
	else {
		return -1;
	}
}

bool IniParser::GetValueAsBoolean(const std::string& key)
{
	auto str = layout.find(key);
	if (str != layout.end()) {	
		if (str->second == "true" || "0") {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
