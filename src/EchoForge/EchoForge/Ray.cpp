#include "Ray.h"
#include "Utils.h"
#include <limits>

tracing::Ray::Ray(Eigen::Vector3f position, Eigen::Vector3f velocity, int unitSize)
	: m_spawnPosition{position}
	, m_currentPosition{ position }
	, m_startPosition{ position }
	, m_velocity{ velocity }
	, m_energy{1.0f,1.0f,1.0f}
	, m_lastSeen{ position }
	, m_lastAxis()
	, m_unitSize{ unitSize }
{
	m_voxelPos = m_currentPosition.array().floor().cast<int>() / unitSize;
	redirect(velocity);
};


void tracing::Ray::loseEnergy(const Eigen::Vector3f& absorption) {
	if (absorption.minCoeff() < 0.0f || absorption.maxCoeff() > 1.0f)
		throw Error::PARAMETER_OUT_OF_BOUNDS;
	m_energy -= absorption.cwiseProduct(m_energy);
};

const Eigen::Vector3f& tracing::Ray::getEnergy() const {
	return m_energy;
};

Eigen::Vector3f tracing::Ray::getPosition() {
	return m_currentPosition;
};


void tracing::Ray::advance() {
	++m_stepCount;
	int axis;

	if (m_tMax.x() < m_tMax.y()) {
		if (m_tMax.x() < m_tMax.z()) axis = 0; 
		else axis = 2;                        
	}
	else {
		if (m_tMax.y() < m_tMax.z()) axis = 1;
		else axis = 2;                         
	}

	float t_at_face = m_tMax(axis);

	m_voxelPos(axis) += m_step(axis);
	m_tMax(axis) += m_tDelta(axis);

	m_currentPosition = m_startPosition + (t_at_face * m_velocity);
	m_lastAxis = axis;
}

Eigen::Vector3i tracing::Ray::getVoxelPos() const {
	return m_voxelPos;
}


Eigen::Vector3f tracing::Ray::getVelocity() const{
	return m_velocity;
};

void tracing::Ray::redirect(const Eigen::Vector3f& newVelocity) {
	m_bouncePositions.push_back(m_currentPosition);
	m_startPosition = m_currentPosition;
	m_velocity = newVelocity;

	const float us = static_cast<float>(m_unitSize);
	m_step = m_velocity.array().sign().cast<int>();

	for (int i = 0; i < 3; ++i) {
		if (m_velocity(i) == 0.0f) {
			m_tDelta(i) = std::numeric_limits<float>::infinity();
			m_tMax(i)   = std::numeric_limits<float>::infinity();
			continue;
		}
		// Time to cross one full voxel (unitSize world units)
		m_tDelta(i) = us / std::abs(m_velocity(i));

		// Use m_voxelPos directly to get exact voxel boundaries in world space
		if (m_velocity(i) > 0) {
			float nextBoundary = (m_voxelPos(i) + 1) * us;
			m_tMax(i) = (nextBoundary - m_currentPosition(i)) / m_velocity(i);
		} else {
			float currBoundary = m_voxelPos(i) * us;
			float distToPrev   = m_currentPosition(i) - currBoundary;
			// If on the boundary, tMax=0 so the ray exits this voxel on the very next advance
			m_tMax(i) = (distToPrev < 1e-6f ? 0.0f : distToPrev / std::abs(m_velocity(i)));
		}
	}
}

Eigen::Vector3f tracing::Ray::getHitNormal() const {
	Eigen::Vector3f n = Eigen::Vector3f::Zero();
	n(m_lastAxis) = -(float)m_step(m_lastAxis);
	return n;
};