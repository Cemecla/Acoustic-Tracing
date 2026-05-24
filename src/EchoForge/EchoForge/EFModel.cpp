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
	m_rayManager{this, 200, 50}
{}
	//m_rayManager(tracing::RayManager(this,20))
	//{}

void EFModel::loadWorld(std::string& filename) {
	json mapfile = loadJson(filename);
	std::string str = "res/settings/materials.json";
	json materialfile = loadJson(str);

	m_world.reset();
	m_world = EFWorldLoader::build(mapfile, materialfile);

	std::vector<int> emitterIds;
	for (auto& emitter : m_world->emitters) {
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

		int emitterId = m_parent->m_view->playSound(m_world->emitters[i].sound_id);
		m_emitterIdMap[m_world->emitters[i].m_id] = emitterId;


		m_scenario.add_event(
			[this, i, emitterId]() {
				if (m_world->emitters[i].loop)
					m_parent->m_view->loopEmitter(emitterId);
				else
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

	if (inputs.rotLeft) yawDelta += rotation_speed * delta;
	if (inputs.rotRight) yawDelta -= rotation_speed * delta;

	if (yawDelta != 0.0f) {
		m_world->avatar.rotateHead(Vector3F(0.0f, yawDelta, 0.0f));
	}

	// Movement (Local)
	Vector3F local(0, 0, 0);
	float speed = m_world->avatar.getSpeed() * delta;

	if (inputs.forward) local.z -= speed;
	if (inputs.backward) local.z += speed;
	if (inputs.left) local.x -= speed;
	if (inputs.right) local.x += speed;

	if (local.x == 0.0f && local.z == 0.0f) return;

	// Movement (World)
	const float yaw = m_world->avatar.getHeadAngles().y;
	const float cos = std::cos(yaw);
	const float sin = std::sin(yaw);

	// World bound
	Vector3F world;
	world.x = local.x * cos + local.z * sin;
	world.z = -local.x * sin + local.z * cos;

	const Map& map = m_world->map;
	const float unitSize = static_cast<float>(map.getUnitSize());
	const float maxX = map.getWidth() * unitSize;
	const float maxZ = map.getDepth() * unitSize;

	const float margin = 50.0; // Should be change later on

	Vector3F currentPos = m_world->avatar.getPosition();
	Vector3F targetPos = currentPos;
	targetPos.x += world.x;
	targetPos.z += world.z;

	if (targetPos.x < margin) targetPos.x = margin;
	if (targetPos.x > maxX - margin) targetPos.x = maxX - margin;
	if (targetPos.z < margin) targetPos.z = margin;
	if (targetPos.z > maxZ - margin) targetPos.z = maxZ - margin;

	world.x = targetPos.x - currentPos.x;
	world.z = targetPos.z - currentPos.z;

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
		float           listenerYaw = m_world->avatar.getHeadAngles().y;

		// DEBUG — À retirer une fois le problème d'azimuth résolu
		constexpr float RAD2DEG = 57.2957795f;
		for (const auto& e : m_world->emitters) {
			Eigen::Vector3f delta = e.position.toEigen() - listenerPos;
			float rawAz = std::atan2(delta.x(), -delta.z()) * RAD2DEG;
			qDebug() << "[AZ-DEBUG] Emitter" << e.m_id
			         << "| emitterPos:" << e.position.x << e.position.z
			         << "| listenerPos:" << listenerPos.x() << listenerPos.z()
			         << "| delta:" << delta.x() << delta.z()
			         << "| rawAz(deg):" << rawAz
			         << "| yaw(deg):" << listenerYaw * RAD2DEG;
		}

		m_acousticInterpreter.feed(batch, listenerPos, listenerYaw);
	}

	const auto& results = m_acousticInterpreter.getResults();
	for (const auto& [id, r] : results) {
		auto it = m_emitterIdMap.find(id);
		if (it == m_emitterIdMap.end()) continue;
		int saeId = it->second;
		m_parent->m_view->updateSoundSource(saeId, r.azimuth, r.elevation, Vector3F::fromEigen(r.gain));
		qDebug() << "[Acoustic] Emitter" << id << "-> SAE id" << saeId
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
	m_emitterIdMap.clear();
	m_world->emitters.clear();
}