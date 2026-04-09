#define _USE_MATH_DEFINES
#include "RayManager.h"
#include <cmath>
#include <algorithm>

#include "Map.h"
#include "Material.h"
#include "EFModel.h"
#include "Vector.h"
using namespace tracing;



RayManager::RayManager(EFModel* parent,int rayCount)
	: m_rayCount(rayCount),
	m_parent(parent),
	m_collisionManager{}
{

	
	// réserve de la mémoire
	m_activeRays->reserve(m_rayCount * 2);
}

const void RayManager::update() {
	Eigen::Vector3f position = m_parent->getAvatar().value().getPosition().toEigen();


	if (m_activeRays->empty()) {
		createRays(position);
	}
	else {
		next_colision();
	}

	//TODO: REFACTOR to single ray
	m_activeRays->erase(
		std::remove_if(m_activeRays->begin(), m_activeRays->end(),
			[](const Ray& r) { return r.getEnergy().maxCoeff() <= 0.01f; }),
		m_activeRays->end()
	);
}



const void RayManager::createRays(Eigen::Vector3f position) {
	float incrementation = M_PI * 2.0f * PHI;
	// From https://devforum.roblox.com/t/generating-equidistant-points-on-a-sphere/874144
	for (int n = 0; n < m_rayCount; n++) {
		float distance = (float)n / m_rayCount;
		float azimuth = incrementation * n;
		float incline = std::acos(1.0f - 2.0f * distance);

		float multiplier = 0.5f;

		float x = std::sin(incline) * std::cos(azimuth) * multiplier;
		float y = std::sin(incline) * std::sin(azimuth) * multiplier;
		float z = std::cos(incline) * multiplier;

		Eigen::Vector3f offset( x, y, z );

		Eigen::Vector3f direction = offset.normalized();
		m_activeRays->emplace_back(position + offset, direction);
	}
}

const void tracing::RayManager::next_colision() {
	const ef::Map& map = m_parent->getMap();

	for (Ray& r : *m_activeRays) {
		r.advance();
		Eigen::Vector3i pos = r.getVoxelPos();
		
		if (!map.isInBound(pos.x(), pos.y(), pos.z())) {
			r.loseEnergy(Eigen::Vector3f(1.0f, 1.0f, 1.0f));
			continue;
		}

		ef::Material mat = map.fetchMaterial(ef::Vector3I::fromEigenI(pos));
		r.loseEnergy(mat.energyLoss.toEigen());

		if (!mat.isPassable) {
			for (const ef::Emitter& emitter : m_parent->getActiveSounds()) {
				if (hasLineOfSight(r.getPosition(), emitter.position.toEigen())) {
					// → créer DTO_RecievedRay et l'ajouter au batch
				}
			}

			auto strategy = m_collisionManager.decision(r, mat.scatteringCoefficient, m_rng);
			Eigen::Vector3f newDir = strategy->getNextDirection(r.getVelocity(), r.getHitNormal(), m_rng);
			r.redirect(newDir);
		}
	}
};

const bool RayManager::hasLineOfSight( const Eigen::Vector3f& from, const Eigen::Vector3f& to) {
// Détecter si un rayon traverse un bloc quand il arrive à une source
	const ef::Map& map = m_parent->getMap();


	Eigen::Vector3f delta = to - from;
	float maxDist = delta.norm();
	if (maxDist < 1e-4f) return true;
	Ray shadowRay(from, delta.normalized());
	Eigen::Vector3i targetVoxel = to.array().floor().cast<int>();

	int maxSteps = static_cast<int>(maxDist * 2.0f) + 5;
	shadowRay.advance();


	for (int i = 0; i < maxSteps; ++i) {
		Eigen::Vector3i pos = shadowRay.getVoxelPos();

		if (pos == targetVoxel) return true;

		if (!map.isInBound(pos.x(), pos.y(), pos.z())) return false;

		ef::Material mat = map.fetchMaterial(ef::Vector3I::fromEigenI(pos));
		if (!mat.isPassable) return false;

		shadowRay.advance();
	}

	return false;
}
