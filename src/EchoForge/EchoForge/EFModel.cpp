#include <fstream>
#include <iostream>

#include "EFModel.h"
#include "EFController.h"
#include "Map.h"



EFModel::EFModel(EFController* parent): m_parent(parent) {

}

void EFModel::update() {

}

json EFModel::loadJson(std::string name) {
	return json{};
}

bool EFModel::saveJson(const json&, std::string name) {
	return true;
}