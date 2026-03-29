#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "EFModel.h"
#include "EFController.h"
#include "Map.h"


// TEMPORARY : m_avatar
// TODO : MAKE numbers of ray configurable
EFModel::EFModel(EFController* parent) :
	m_parent(parent),
	m_map(Map(5, 5, 5)),
	m_rayManager{this,10}
{
}
	//m_rayManager(tracing::RayManager(this,20))
	//{}

void EFModel::update() {

	// Defensive
	if (!m_avatar.has_value()) return;
	m_rayManager.update();


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

const std::optional<Avatar>& EFModel::getAvatar() const {
	return m_avatar;
};
