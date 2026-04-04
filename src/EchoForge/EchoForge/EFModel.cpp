#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "EFModel.h"
#include "EFController.h"
#include "Map.h"
#include "EFWorldLoader.h"
#include "Utils.h"

using namespace ef;

// TEMPORARY : m_avatar
// TODO : MAKE numbers of ray configurable
EFModel::EFModel(EFController* parent) :
	m_parent(parent),
	m_rayManager{this,10}
{}
	//m_rayManager(tracing::RayManager(this,20))
	//{}

void EFModel::loadWorld(std::string& filename) {
	json mapfile = loadJson(filename);
	m_world = EFWorldLoader::build(mapfile);
}

void EFModel::update(float delta) {

	// Defensive
	if (!m_world.has_value()) return;
	m_rayManager.update();

}

json EFModel::loadJson(std::string& filename) {
	std::ifstream file(filename);

	if (!file.is_open())
	{
		throw Error::FILE_NOT_FOUND;
	}

	std::ostringstream data;

	data << file.rdbuf();

	file.close();

	std::string strdata = data.str();

	return json::parse(strdata);
}

bool EFModel::saveJson(const json&, std::string& filename) {
	return true;
}

const std::optional<Avatar>& EFModel::getAvatar() const {
	return m_world->avatar;
};

const Map& EFModel::getMap() const {
	return m_world->map; 
}