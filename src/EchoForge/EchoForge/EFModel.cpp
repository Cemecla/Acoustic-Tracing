#include "EFModel.h"
#include "EFController.h"
#include <fstream>
#include <iostream>


EFModel::EFModel(EFController* parent): m_parent(parent) 
{
}

json EFModel::loadJson(std::string name) {
	return json{};
}

bool EFModel::saveJson(json data, std::string name) {
	return true;
}