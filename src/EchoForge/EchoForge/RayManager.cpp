#include "RayManager.h"
#include <cmath>
#include <numbers>
#include <ranges>

#include "Map.h"
#include "Material.h"
#include "EFModel.h"
#include "Vector.h"

using namespace tracing;

RayManager::RayManager(EFModel* parent, int rayCount, int raysPerFrame)
	: m_rayCount(rayCount),
	m_raysPerFrame(raysPerFrame),
	m_parent(parent),
	m_collisionManager{}
{
	m_activeRays->reserve(m_rayCount * 2);
}

const void RayManager::update() {
	Eigen::Vector3f position = m_parent->getAvatar().value().getPosition().toEigen();

	next_colision();

	std::erase_if(*m_activeRays, [](const Ray& r) {
		return r.getEnergy().maxCoeff() <= 0.01f;
	});

	int slots = m_rayCount - static_cast<int>(m_activeRays->size());
	if (slots > 0)
		createRays(position, std::min(m_raysPerFrame, slots));
}

const void RayManager::createRays(Eigen::Vector3f position, int count) {
	constexpr float incrementation = std::numbers::pi_v<float> * 2.0f * std::numbers::phi_v<float>;
	int unitSize = m_parent->getMap().getUnitSize();
	// From https://devforum.roblox.com/t/generating-equidistant-points-on-a-sphere/874144
	for (int i = 0; i < count; ++i) {
		int n = m_nextRayIndex % m_rayCount;
		float distance = static_cast<float>(n) / m_rayCount;
		float azimuth  = incrementation * n;
		float incline  = std::acos(1.0f - 2.0f * distance);

		constexpr float multiplier = 0.5f;

		float x = std::sin(incline) * std::cos(azimuth) * multiplier;
		float y = std::sin(incline) * std::sin(azimuth) * multiplier;
		float z = std::cos(incline) * multiplier;

		Eigen::Vector3f offset(x, y, z);
		Eigen::Vector3f direction = offset.normalized();
		m_activeRays->emplace_back(position, direction, unitSize);
		++m_nextRayIndex;
	}
}

int tracing::RayManager::checkLosToEmitters(const Ray& r, int& budgetRemaining, bool isDirect) {
	const auto& allEmitters = m_parent->getEmitters();
	constexpr float LOS_ENERGY_THRESHOLD = 0.02f;

	if (r.getEnergy().maxCoeff() < LOS_ENERGY_THRESHOLD) return 0;
	if (budgetRemaining <= 0) return 0;

	// Vérifie d'abord que le point de rebond est dans le MÊME ESPACE ACOUSTIQUE que l'avatar.
	// Un rayon qui a traversé un mur par transmission se retrouve de l'autre côté :
	// depuis là, il peut avoir LOS vers l'emitter, mais le son ne peut pas repartir
	// vers l'avatar sans re-traverser le mur. Ce check filtre ces faux positifs.
	--budgetRemaining;
	const Eigen::Vector3f avatarPos = m_parent->getAvatar().value().getPosition().toEigen();
	if (!hasLineOfSight(r.getPosition(), avatarPos)) return 0;

	int checked = 0;
	for (const auto& emitter : allEmitters) {
		if (!emitter.isActive)    continue;
		if (budgetRemaining <= 0) break;
		--budgetRemaining;
		++checked;

		if (!hasLineOfSight(r.getPosition(), emitter.position.toEigen())) continue;

		// lastSeen = position du rebond (mur visible depuis l'avatar et depuis l'emitter).
		// Direction acoustique correcte : le son "entre" dans la pièce depuis ce mur.
		m_receivedRays.emplace_back(
			r.getPosition(),
			r.getEnergy(),
			r.getStepCount(),
			static_cast<std::uint8_t>(emitter.m_id),
			r.getBouncePositions(),
			r.getVelocity(),
			isDirect
		);
	}
	return checked;
}

const void tracing::RayManager::next_colision() {
	const ef::Map& map         = m_parent->getMap();
	const auto&    allEmitters = m_parent->getEmitters();

	// Budget LOS par frame : 4 checks par emitter actif, minimum 30.
	// Évite d'épuiser le budget sur les premières rays quand il y a beaucoup d'emitters.
	int activeEmitterCount = static_cast<int>(
		std::ranges::count_if(allEmitters, [](const ef::Emitter& e){ return e.isActive; })
	);
	int losRemaining = std::max(30, activeEmitterCount * 4 * static_cast<int>(m_activeRays->size()) / 10);

	for (Ray& r : *m_activeRays) {
		r.advance();
		Eigen::Vector3i pos = r.getVoxelPos();

		// --- Hors bounds : DTO ambient ---
		if (!map.isInBound(pos.x(), pos.y(), pos.z())) {
			m_receivedRays.emplace_back(
				r.getPosition(),
				r.getEnergy(),
				r.getStepCount(),
				AMBIENT_EMITTER_ID,
				r.getBouncePositions(),
				r.getVelocity(),
				false
			);
			r.loseEnergy(Eigen::Vector3f(1.0f, 1.0f, 1.0f));
			continue;
		}

		ef::Material mat = map.fetchMaterial(ef::Vector3I::fromEigenI(pos));
		r.loseEnergy(mat.energyLoss.toEigen());

		// --- Son direct : le ray passe par le voxel d'un emitter actif ---
		for (const auto& emitter : allEmitters) {
			if (!emitter.isActive) continue;
			Eigen::Vector3i emitterVoxel = (emitter.position.toEigen().array() / map.getUnitSize()).floor().cast<int>();
			if (pos != emitterVoxel) continue;

			m_receivedRays.emplace_back(
				emitter.position.toEigen(),   // position exacte de l'emitter
				r.getEnergy(),
				r.getStepCount(),
				static_cast<std::uint8_t>(emitter.m_id),
				r.getBouncePositions(),
				r.getVelocity(),
				true                          // isDirect = true
			);
		}

		// --- Contact avec un solide ---
		if (!mat.isPassable) {
			if (!r.wasInSolid()) {
				// Première entrée dans le solide : LOS check + décision de stratégie
				checkLosToEmitters(r, losRemaining, false);

				auto strategy   = m_collisionManager.decision(r, mat.scatteringCoefficient, mat.transmissionCoefficient, m_rng);
				Eigen::Vector3f newDir = strategy->getNextDirection(r.getVelocity(), r.getHitNormal(), m_rng);
				r.redirect(newDir);
				r.setInSolid(strategy->isTransmission());
			} else {
				// Transmission en cours : LOS depuis l'intérieur du mur
				// (le son peut "sortir" vers un emitter de l'autre côté)
				checkLosToEmitters(r, losRemaining, false);
				r.setInSolid(true);
			}
		} else {
			r.setInSolid(false);
		}
	}
}

const std::vector<DTO_RecievedRay>& RayManager::getReceivedRays() const {
	return m_receivedRays;
}


void RayManager::clearReceivedRays() {
	m_receivedRays.clear();
}

void RayManager::clearAllRays() {
	m_receivedRays.clear();
	m_activeRays->clear();
	m_nextRayIndex = 0;
}


const bool RayManager::hasLineOfSight(const Eigen::Vector3f& from, const Eigen::Vector3f& to) {
// Détecter si un rayon traverse un bloc quand il arrive à une source
	const ef::Map& map = m_parent->getMap();

	Eigen::Vector3f delta = to - from;
	float maxDist = delta.norm();
	if (maxDist < 1e-4f) return true;
	Ray shadowRay(from, delta.normalized(),map.getUnitSize());
	// targetVoxel doit être en coordonnées voxel (world / unitSize), pas en world units.
	// Avant le fix de EFWorldLoader, emitter.position était déjà en voxel units donc ça
	// marchait par accident. Maintenant les deux sont en world units → conversion explicite.
	const float us = static_cast<float>(map.getUnitSize());
	Eigen::Vector3i targetVoxel = (to.array() / us).floor().cast<int>();

	int maxSteps = static_cast<int>(maxDist * 2.0f) + 5;
	shadowRay.advance();

	for (int i : std::views::iota(0, maxSteps)) {
		Eigen::Vector3i pos = shadowRay.getVoxelPos();

		if (pos == targetVoxel) return true;

		if (!map.isInBound(pos.x(), pos.y(), pos.z())) return false;

		ef::Material mat = map.fetchMaterial(ef::Vector3I::fromEigenI(pos));
		if (!mat.isPassable) return false;

		shadowRay.advance();
	}

	return false;
}
