#pragma once
#include <Eigen/Dense>
#include <cstdint>
#include <vector>

#ifndef DTO_RECIEVEDRAY_H
	#define DTO_RECIEVEDRAY_H

class DTO_RecievedRay
{
public:
	DTO_RecievedRay(Eigen::Vector3f lastSeen, Eigen::Vector3f energy, int stepCount, std::uint8_t emitterID, std::vector<Eigen::Vector3f> bouncePoints)
		: m_lastSeen(lastSeen), m_energy(energy), m_stepCount(stepCount), m_emitterID(emitterID), m_bouncePoints(std::move(bouncePoints)) {}

	const Eigen::Vector3f&              getLastSeen()     const { return m_lastSeen; }
	const Eigen::Vector3f&              getEnergy()       const { return m_energy; }
	int                                 getStepCount()    const { return m_stepCount; }
	std::uint8_t                        getEmitterID()    const { return m_emitterID; }
	const std::vector<Eigen::Vector3f>& getBouncePoints() const { return m_bouncePoints; }

private:
	Eigen::Vector3f              m_lastSeen;
	Eigen::Vector3f              m_energy;
	int                          m_stepCount;
	std::uint8_t                 m_emitterID;
	std::vector<Eigen::Vector3f> m_bouncePoints;
};

#endif // !DTO_RECIEVEDRAY_H
