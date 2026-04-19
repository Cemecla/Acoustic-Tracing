#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <QDebug>

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
	m_rayManager{this,100,50}
{}
	//m_rayManager(tracing::RayManager(this,20))
	//{}

void EFModel::loadWorld(std::string& filename) {
	json mapfile = loadJson(filename);
	// HARDCODED change later
	std::string str = "../../../settings/materials.json";
	// HARDCODED change later
	json materialfile = loadJson(str);
	m_world = EFWorldLoader::build(mapfile, materialfile);

	// TODO: remplacer par le chargement depuis le JSON (scenarioItem)
	ef::Emitter testEmitter{ 0, ef::Vector3F(5.0f, 1.0f, 5.0f), 1.0f, true };
	m_world->emitters.push_back(testEmitter);
}

void EFModel::update(float delta) {

	// Defensive
	if (!m_world.has_value()) return;
	m_rayManager.update();
	processBatch();

}

void EFModel::processBatch() {
	const auto& batch = m_rayManager.getReceivedRays();
	// Test généré par IA
	// Throttle : log toutes les 60 frames pour ne pas spammer
	constexpr int LOG_INTERVAL = 10;
	if (++m_debugFrameCount % LOG_INTERVAL != 0) {
		m_rayManager.clearReceivedRays();
		return;
	}

	if (batch.empty()) {
		qDebug() << "[RayDebug] Frame" << m_debugFrameCount << "| Aucun rayon reçu";
		m_rayManager.clearReceivedRays();
		return;
	}

	// Grouper par emitter pour un résumé lisible
	std::map<uint8_t, std::vector<const DTO_RecievedRay*>> byEmitter;
	for (const auto& ray : batch)
		byEmitter[ray.getEmitterID()].push_back(&ray);

	const bool hasAmbient = byEmitter.contains(tracing::RayManager::AMBIENT_EMITTER_ID);
	qDebug() << "[RayDebug] Frame" << m_debugFrameCount
	         << "| Total reçus :" << batch.size()
	         << "| Emitters actifs :" << (byEmitter.size() - (hasAmbient ? 1 : 0))
	         << "| Rayons sortis (ambiant) :" << (hasAmbient ? byEmitter.at(tracing::RayManager::AMBIENT_EMITTER_ID).size() : 0);

	for (const auto& [emitterID, rays] : byEmitter) {
		if (emitterID == tracing::RayManager::AMBIENT_EMITTER_ID) continue;
		Eigen::Vector3f avgEnergy = Eigen::Vector3f::Zero();
		float avgSteps = 0.f;

		for (const auto* r : rays) {
			avgEnergy += r->getEnergy();
			avgSteps  += static_cast<float>(r->getStepCount());
		}
		avgEnergy /= static_cast<float>(rays.size());
		avgSteps  /= static_cast<float>(rays.size());

		qDebug() << "  Emitter" << emitterID
		         << "| rayons :" << rays.size()
		         << "| énergie moy (H/M/L) :"
		         << avgEnergy.x() << "/" << avgEnergy.y() << "/" << avgEnergy.z()
		         << "| steps moy :" << avgSteps;
	}

	m_rayManager.clearReceivedRays();
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
}

const Avatar& EFModel::getAvatar(int temporary) const {
	return m_world->avatar;
}

const Map& EFModel::getMap() const {
	return m_world->map; 
}

const std::vector<ef::Emitter>& EFModel::getEmitters() const {
	return m_world->emitters;
}

std::vector<ef::Emitter> EFModel::getActiveSounds() {
	if (!m_world.has_value()) return {};

	std::vector<ef::Emitter> result;
	std::copy_if(m_world->emitters.begin(), m_world->emitters.end(),
		std::back_inserter(result),
		[](const ef::Emitter& s) { return s.isActive; });

	return result;
}