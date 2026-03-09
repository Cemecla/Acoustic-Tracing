#include "Ray.h"
#include "Utils.h"


tracing::Ray::Ray(ef::Vector3 position, ef::Vector3 velocity)
	: m_startPosition{ position }
	, m_velocity{ velocity }
	, m_energy(1.0f)
	, m_currentPosition{ position }
	, m_lastSeen{ position }
	, m_time{ std::chrono::high_resolution_clock::now() }
{
};


void tracing::Ray::loseEnergy(const float& absorption) {
	if (absorption > 0 && absorption > 1)
		throw Error::PARAMETER_OUT_OF_BOUNDS;
	m_energy -= absorption * m_energy;
};

float tracing::Ray::getEnergy() {
	return m_energy;
};

ef::Vector3 tracing::Ray::getPosition() {
	return m_currentPosition;
};

int tracing::Ray::getTimeDelta() {
	auto end = std::chrono::high_resolution_clock::now();
	auto res = end - m_time;

	return static_cast<int>(res.count());
};

void tracing::Ray::m_next() {
	//TODO : DDA algorythme
}



