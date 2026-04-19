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
	createRays(position, m_raysPerFrame);

	std::erase_if(*m_activeRays, [](const Ray& r) {
		return r.getEnergy().maxCoeff() <= 0.01f;
	});
}



const void RayManager::createRays(Eigen::Vector3f position, int count) {
	constexpr float incrementation = std::numbers::pi_v<float> * 2.0f * std::numbers::phi_v<float>;
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
		m_activeRays->emplace_back(position + offset, direction);
		++m_nextRayIndex;
	}
}

const void tracing::RayManager::next_colision() {
	const ef::Map& map             = m_parent->getMap();
	const auto&    allEmitters     = m_parent->getEmitters();

	for (Ray& r : *m_activeRays) {
		r.advance();
		Eigen::Vector3i pos = r.getVoxelPos();

		if (!map.isInBound(pos.x(), pos.y(), pos.z())) {
			m_receivedRays.emplace_back(
				r.getPosition(),
				r.getEnergy(),
				r.getStepCount(),
				AMBIENT_EMITTER_ID,
				r.getBouncePositions()
			);
			r.loseEnergy(Eigen::Vector3f(1.0f, 1.0f, 1.0f));
			continue;
		}

		ef::Material mat = map.fetchMaterial(ef::Vector3I::fromEigenI(pos));
		r.loseEnergy(mat.energyLoss.toEigen());

		if (!mat.isPassable) {
			if (!r.wasInSolid()) {  // surface entry → décision unique
				auto visibleEmitters = allEmitters
					| std::views::filter([](const ef::Emitter& e) { return e.isActive; })
					| std::views::filter([&](const ef::Emitter& e) {
						return hasLineOfSight(r.getPosition(), e.position.toEigen());
					  });

				for (const auto& emitter : visibleEmitters) {
					m_receivedRays.emplace_back(
						r.getPosition(),
						r.getEnergy(),
						r.getStepCount(),
						static_cast<std::uint8_t>(emitter.sound_id),
						r.getBouncePositions()
					);
				}

				auto strategy = m_collisionManager.decision(r, mat.scatteringCoefficient, m_rng);
				Eigen::Vector3f newDir = strategy->getNextDirection(r.getVelocity(), r.getHitNormal(), m_rng);
				r.redirect(newDir);
			}
			// else : intérieur du mur → transmission forcée, le rayon continue sans redirection
			r.setInSolid(true);
		} else {
			r.setInSolid(false);
		}
	}
}

const std::vector<DTO_RecievedRay>& RayManager::getReceivedRays() const {
	return m_receivedRays;
}

std::vector<std::vector<ef::Vector3F>> RayManager::getRayPaths() const {
	std::vector<std::vector<ef::Vector3F>> paths;
	paths.reserve(m_activeRays->size());

	for (const Ray& r : *m_activeRays) {
		const auto& bounces = r.getBouncePositions();
		auto& path = paths.emplace_back();
		path.reserve(bounces.size());

		for (const Eigen::Vector3f& p : bounces)
			path.push_back(ef::Vector3F::fromEigen(p));
	}
	return paths;
}

void RayManager::clearReceivedRays() {
	m_receivedRays.clear();
}

const bool RayManager::hasLineOfSight(const Eigen::Vector3f& from, const Eigen::Vector3f& to) {
// Détecter si un rayon traverse un bloc quand il arrive à une source
	const ef::Map& map = m_parent->getMap();

	Eigen::Vector3f delta = to - from;
	float maxDist = delta.norm();
	if (maxDist < 1e-4f) return true;
	Ray shadowRay(from, delta.normalized());
	Eigen::Vector3i targetVoxel = to.array().floor().cast<int>();

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
