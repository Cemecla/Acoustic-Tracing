#pragma once

#include "json.hpp"
#include <string>

using json = nlohmann::json;

class EFController;

class EFModel
{
	public:
		EFModel(EFController* parent);
		json loadJson(std::string name);
		bool saveJson(json data, std::string name);

		
	
	private:
		EFController* m_parent;
};
