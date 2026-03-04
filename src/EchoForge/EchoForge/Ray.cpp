#include "Ray.h"
#include "Error.h"


Tracing::Ray::Ray(Vector::Vector3 position, Vector::Vector3 velocity)
	: m_startPosition{ position }
	, m_velocity{ velocity }
	, m_energy(1.0f)
	, m_currentPosition{ position }
	, m_lastSeen{ position }
	, m_time{ std::chrono::high_resolution_clock::now() }
{
};


void Tracing::Ray::loseEnergy(const float& absorption) {
	if (absorption > 0 && absorption > 1)
		throw Error::PARAMETER_OUT_OF_BOUNDS;
	m_energy -= absorption * m_energy;
};

float Tracing::Ray::getEnergy() {
	return m_energy;
};

Vector::Vector3 Tracing::Ray::getPosition() {
	return m_currentPosition;
};

int Tracing::Ray::getTimeDelta() {
	auto end = std::chrono::high_resolution_clock::now();
	auto res = end - m_time;

	return static_cast<int>(res.count());
};



