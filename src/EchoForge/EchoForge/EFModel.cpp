#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <chrono>
#include <cmath>
#include <QDebug>

#include "EFModel.h"
#include "EFView.h"
#include "EFController.h"
#include "Map.h"
#include "EFWorldLoader.h"
#include "Utils.h"

using namespace ef;

// TEMPORARY : m_avatar
// TODO : MAKE numbers of ray configurable
EFModel::EFModel(EFController* parent) :
	m_parent(parent),
	m_rayManager{this, 20, 2}
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

	buildScenario();
}

void EFModel::buildScenario() {
	if (!m_world.has_value()) return;

	const auto startTime = std::chrono::steady_clock::now();

	for (int i = 0; i < static_cast<int>(m_world->emitters.size()); ++i) {
		const auto& em = m_world->emitters[i];

		if (em.eventType == EventType::TIME) {
			float triggerTime = em.triggerTime;
			m_scenario.add_event(
				[this, i]() {
					m_world->emitters[i].isActive = true;
					m_parent->activateEmitter(i);
					qDebug() << "[Scenario] Emetteur" << i << "activé (timer)";
				},
				[this, i, triggerTime, startTime]() -> bool {
					if (!m_world.has_value()) return false;
					if (m_world->emitters[i].isActive) return false;
					auto elapsed = std::chrono::steady_clock::now() - startTime;
					return std::chrono::duration<float>(elapsed).count() >= triggerTime;
				}
			);
		}
		else if (em.eventType == EventType::COLLISION) {
			ef::Vector3F emitterPos = em.position;
			ef::Vector3F delta      = em.triggerDelta;
			m_scenario.add_event(
				[this, i]() {
					m_world->emitters[i].isActive = true;
					m_parent->activateEmitter(i);
					qDebug() << "[Scenario] Emetteur" << i << "activé (collision)";
				},
				[this, i, emitterPos, delta]() -> bool {
					if (!m_world.has_value()) return false;
					if (m_world->emitters[i].isActive) return false;
					const auto& avatarPos = m_world->avatar.getPosition();
					return std::abs(avatarPos.x - emitterPos.x) <= delta.x
						&& std::abs(avatarPos.y - emitterPos.y) <= delta.y
						&& std::abs(avatarPos.z - emitterPos.z) <= delta.z;
				}
			);
		}
	}
}


void EFModel::startScenario() {
	m_scenario.start();
}

void EFModel::update(float delta) {

	// Defensive
	if (!m_world.has_value()) return;
	Inputs inputs = m_parent->getLastInputs();
	moveAvatar(inputs, delta);

	m_rayManager.update();
	m_parent->updateViewRays();
	m_rayManager.clearReceivedRays();

	// Mise à jour spatiale des emitters actifs
	const auto& avatarPos   = m_world->avatar.getPosition();
	const auto& headAngles  = m_world->avatar.getHeadAngles();

	for (const auto& emitter : m_world->emitters) {
		if (!emitter.isActive || emitter.audioEmitterId == -1) continue;

		ef::Vector3F dir = emitter.position - avatarPos;

		float rawAzimuth = std::atan2(dir.x, -dir.z) * (180.0f / M_PI);
		float azimuth    = rawAzimuth - headAngles.y;

		float horizontal = std::sqrt(dir.x * dir.x + dir.z * dir.z);
		float elevation  = std::atan2(dir.y, horizontal) * (180.0f / M_PI);

		m_parent->updateEmitterListener(emitter.audioEmitterId, azimuth, elevation);
	}
}

void EFModel::moveAvatar(Inputs& inputs, float delta) {
	Vector3F direction(0, 0, 0);
	float speed = m_world->avatar.getSpeed() * delta;
	if (inputs.forward)  direction.z -= speed;
	if (inputs.backward) direction.z += speed;
	if (inputs.left)     direction.x -= speed;
	if (inputs.right)    direction.x += speed;
	if (direction.x != 0 || direction.z != 0)
		m_world->avatar.move(direction);
}

const std::vector<DTO_RecievedRay>& EFModel::getDTORays() const {
	return m_rayManager.getReceivedRays();
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

std::vector<ef::Emitter>& EFModel::getEmittersRef() {
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
