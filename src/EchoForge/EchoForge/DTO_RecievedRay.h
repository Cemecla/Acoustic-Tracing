#pragma once
#include <Eigen/Dense>
#include <cstdint>
#include <vector>

#ifndef DTO_RECIEVEDRAY_H
	#define DTO_RECIEVEDRAY_H

class DTO_RecievedRay
{
public:
	DTO_RecievedRay(Eigen::Vector3f lastSeen, Eigen::Vector3f energy, int stepCount, std::uint8_t emitterID, std::vector<Eigen::Vector3f> bouncePoints, Eigen::Vector3f direction, Eigen::Vector3f emitterPosition)
		: m_lastSeen(lastSeen), m_energy(energy), m_stepCount(stepCount), m_emitterID(emitterID), m_bouncePoints(std::move(bouncePoints)), m_direction(direction), m_emitterPosition(emitterPosition) {}

	const Eigen::Vector3f& getLastSeen()        const { return m_lastSeen; }
	const Eigen::Vector3f& getEnergy()          const { return m_energy; }
	int                    getStepCount()       const { return m_stepCount; }
	std::uint8_t           getEmitterID()       const { return m_emitterID; }
	const std::vector<Eigen::Vector3f>& getBouncePoints() const { return m_bouncePoints; }
	const Eigen::Vector3f& getDirection()       const { return m_direction; }
	const Eigen::Vector3f& getEmitterPosition() const { return m_emitterPosition; }

private:
	Eigen::Vector3f              m_lastSeen;
	Eigen::Vector3f              m_energy;
	int                          m_stepCount;
	std::uint8_t                 m_emitterID;
	std::vector<Eigen::Vector3f> m_bouncePoints;
	Eigen::Vector3f              m_direction;
	Eigen::Vector3f              m_emitterPosition;
};

#endif // !DTO_RECIEVEDRAY_H
