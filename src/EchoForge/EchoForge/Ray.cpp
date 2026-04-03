#include "Ray.h"
#include "Utils.h"
#include <limits>

tracing::Ray::Ray(Eigen::Vector3f position, Eigen::Vector3f velocity)
	: m_spawnPosition{position}
	, m_currentPosition{ position }
	, m_startPosition{ position }
	, m_velocity{ velocity }
	, m_energy{1.0f,1.0f,1.0f}
	, m_lastSeen{ position }
	, m_time{ std::chrono::high_resolution_clock::now() }
	, m_lastAxis()
{
	m_voxelPos = m_currentPosition.array().floor().cast<int>();

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

int tracing::Ray::getTimeDelta() {
	auto end = std::chrono::high_resolution_clock::now();
	auto res = end - m_time;

	return static_cast<int>(res.count());
};

void tracing::Ray::advance() {
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
	m_startPosition = m_currentPosition;
	m_velocity = newVelocity;

	m_step = m_velocity.array().sign().cast<int>();
	m_tDelta = m_velocity.array().abs().inverse();
	

	for (int i = 0; i < 3; ++i) {
		if (m_velocity(i) > 0) {
			m_tMax(i) = (std::floor(m_currentPosition(i) + 1.0f) - m_currentPosition(i)) * m_tDelta(i);
		}
		else if (m_velocity(i) < 0) {
			m_tMax(i) = (m_currentPosition(i) - std::floor(m_currentPosition(i))) * m_tDelta(i);
		}
		else {
			m_tMax(i) = std::numeric_limits<float>::infinity();
		}
	}

}

Eigen::Vector3f tracing::Ray::getHitNormal() const {
	Eigen::Vector3f n = Eigen::Vector3f::Zero();
	n(m_lastAxis) = -(float)m_step(m_lastAxis);
	return n;
};