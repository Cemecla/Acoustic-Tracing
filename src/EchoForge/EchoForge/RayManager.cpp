#define _USE_MATH_DEFINES
#include "RayManager.h"
#include <cmath>
#include <algorithm>

#include "EFModel.h"
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
	ef::Vector3F avatar = m_parent->getAvatar().value().getPosition();



	Eigen::Vector3f position;
	position << avatar.x, avatar.y, avatar.z;


	if (m_activeRays->empty()) {
		createRays(position);
	}
	else {
		next_colision();
	}

	//TODO: REFACTOR to single ray
	m_activeRays->erase(
		std::remove_if(m_activeRays->begin(), m_activeRays->end(),
			[](const Ray& r) { return r.getEnergy() <= 0.0f; }),
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

};
