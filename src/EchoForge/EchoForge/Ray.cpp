#include "Ray.h"
#include "Error.h"


Ray::Ray(std::vector<float> position, std::vector<float> velocity)
	: m_startPosition{ position }
	, m_velocity{ velocity }
	, m_energy(1.0f)
	, m_currentPosition{ position }
	, m_lastSeen{ position }
	, m_time{ std::chrono::high_resolution_clock::now() }
{
};


void Ray::loseEnergy(float absorption) {
	if (absorption > 0 && absorption > 1)
		throw Error::OUT_OF_BOUNDS; // Erreure de TypeError
	m_energy -= absorption * m_energy;
};

float Ray::getEnergy() {
	return m_energy;
};

std::vector<float> Ray::getPosition() {
	return m_currentPosition;
};

int Ray::getTimeDelta() {
	auto end = std::chrono::high_resolution_clock::now();
	auto res = end - m_time;

	return static_cast<int>(res.count());
};



