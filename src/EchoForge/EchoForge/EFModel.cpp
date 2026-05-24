#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <chrono>
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
	m_rayManager{this, 500, 80}
{}
	//m_rayManager(tracing::RayManager(this,20))
	//{}

void EFModel::loadWorld(std::string& filename) {
	json mapfile = loadJson(filename);
	std::string str = "res/settings/materials.json";
	json materialfile = loadJson(str);

	m_world.reset();
	m_world = EFWorldLoader::build(mapfile, materialfile);

	m_acousticInterpreter.setUnitSize(static_cast<float>(m_world->map.getUnitSize()));

	std::vector<int> emitterIds;
	for (auto& emitter : m_world->emitters) {
		emitter.sound_id = m_parent->m_view->registerSound(emitter.filename);
		emitterIds.push_back(emitter.m_id);
	}
	m_acousticInterpreter.setKnownEmitters(emitterIds);

	buildScenario();
}

void EFModel::buildScenario() {
	if (!m_world.has_value()) return;

	const auto startTime = std::chrono::steady_clock::now();

	for (int i = 0; i < static_cast<int>(m_world->emitters.size()); ++i) {
		if (m_world->emitters[i].eventType != EventType::TIME) continue;

		float triggerTime = m_world->emitters[i].triggerTime;

		// Créer la source audio avec gain 0 pour éviter le burst sonore au démarrage
		ef::Vector3F zeroGain(0.f, 0.f, 0.f);
		int emitterId = m_parent->m_view->playSound(m_world->emitters[i].sound_id, 0.f, 0.f, zeroGain);

		m_scenario.add_event(
			[this, i, emitterId]() {
				m_parent->m_view->resumeEmitter(emitterId);
				m_world->emitters[i].isActive = true;
				qDebug() << "[Scenario] Emetteur" << i << "activé";
			},
			[this, i, triggerTime, startTime]() -> bool {
				if (!m_world.has_value()) return false;
				if (m_world->emitters[i].isActive) return false;
				auto elapsed = std::chrono::steady_clock::now() - startTime;
				return std::chrono::duration<float>(elapsed).count() >= triggerTime;
			}
		);
	}
}


void EFModel::startScenario() {
	m_scenario.start();
}

void EFModel::stopScenario() {
	m_scenario.stop();
}

void EFModel::update(float delta) {

	// Defensive
	if (!m_world.has_value()) return;
	Inputs inputs = m_parent->getLastInputs();
	moveAvatar(inputs, delta);

	m_rayManager.update();
	processBatch();

	m_updateRayTimer += delta;
	if (m_updateRayTimer >= 1.0)
	{
		m_parent->updateViewRays();
		m_updateRayTimer = 0.0;
	}
	m_rayManager.clearReceivedRays();

}

void EFModel::moveAvatar(Inputs& inputs, float delta) {
	if (!m_world.has_value()) return;

	// Rotation
	float rotation_speed = m_world->avatar.getRotationSpeed();
	float yawDelta = 0.0f;

	if (inputs.rotLeft)  yawDelta += rotation_speed * delta;
	if (inputs.rotRight) yawDelta -= rotation_speed * delta;

	if (yawDelta != 0.0f) {
		m_world->avatar.rotateHead(Vector3F(0.0f, yawDelta, 0.0f));
	}

	// Movement (Local)
	Vector3F local(0, 0, 0);
	float speed = m_world->avatar.getSpeed() * delta;

	if (inputs.forward)  local.z -= speed;
	if (inputs.backward) local.z += speed;
	if (inputs.left)     local.x -= speed;
	if (inputs.right)    local.x += speed;

	if (local.x == 0.0f && local.z == 0.0f) return;

	// Movement (World)
	const float yaw = m_world->avatar.getHeadAngles().y;
	const float cos = std::cos(yaw);
	const float sin = std::sin(yaw);

	Vector3F world;
	world.x = local.x * cos + local.z * sin;
	world.z = -local.x * sin + local.z * cos;

	m_world->avatar.move(world);
}

const std::vector<DTO_RecievedRay>& EFModel::getDTORays() const {
	return m_rayManager.getReceivedRays();
}

void EFModel::processBatch() {
	const auto& batch = m_rayManager.getReceivedRays();

	if (m_world.has_value())
	{
		Eigen::Vector3f listenerPos = m_world->avatar.getPosition().toEigen();
		m_acousticInterpreter.feed(batch, listenerPos);
	}

	const auto& results = m_acousticInterpreter.getResults();
	for (const auto& [id, r] : results) {
		m_parent->m_view->updateSoundSource(id, r.azimuth, r.elevation, Vector3F::fromEigen(r.gain));
		qDebug() << "[Acoustic] Emitter" << id
		         << "| az:" << r.azimuth << "el:" << r.elevation
		         << "| gain HMF:" << r.gain.x() << r.gain.y() << r.gain.z();
	}


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

bool EFModel::saveJson(const json& data, std::string& filename) {
	std::ofstream file(filename);

	if (!file.is_open())
	{
		throw Error::FILE_NOT_FOUND;
	}

	file << data.dump(4);

	file.close();

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


void EFModel::endWorld() {
	m_rayManager.clearAllRays();
	m_acousticInterpreter.clear();
	m_scenario.stop();
	m_scenario.clearEvent();
	m_world->emitters.clear();
}