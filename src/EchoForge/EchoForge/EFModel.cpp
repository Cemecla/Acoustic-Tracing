#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "EFModel.h"
#include "EFController.h"
#include "Map.h"



EFModel::EFModel(EFController* parent): m_parent(parent), m_map(Map(5, 5, 5)) {
	
}

void EFModel::update() {

}

json EFModel::loadJson(std::string& name) {
	std::ifstream file(name);

	if (!file.is_open())
	{
		return json{};
	}

	std::ostringstream data;

	data << file.rdbuf();

	file.close();

	std::string strdata = data.str();

	return json::parse(strdata);
}

bool EFModel::saveJson(const json&, std::string& name) {
	return true;
}