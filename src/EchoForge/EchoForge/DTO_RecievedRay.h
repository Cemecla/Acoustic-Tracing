#pragma once
#include "Vector.h"
#include <cstdint>

#ifndef DTO_RECIEVEDRAY_H
	#define DTO_RECIEVEDRAY_H

class DTO_RecievedRay
{
public:
	DTO_RecievedRay(ef::Vector3F lastSeen, ef::Vector3F energy, int timeDelta, std::uint8_t emitterID)
		: m_lastSeen(lastSeen), m_energy(energy), m_timeDelta(timeDelta), m_emitterID(emitterID) {}

	const ef::Vector3F& getLastSeen() const { return m_lastSeen; }
	const ef::Vector3F& getEnergy()   const { return m_energy; }
	int                 getTimeDelta() const { return m_timeDelta; }
	std::uint8_t        getEmitterID() const { return m_emitterID; }

private:
	ef::Vector3F    m_lastSeen;
	ef::Vector3F    m_energy;
	int             m_timeDelta;
	std::uint8_t    m_emitterID;
};

#endif // !DTO_RECIEVEDRAY_H
