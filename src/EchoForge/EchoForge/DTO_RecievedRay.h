#pragma once
#include "Vector.h"
#include <cstdint>
#ifndef DTO_RECIEVEDRAY_H
	#define DTO_RECIEVEDRAY_H
class DOT_RecievedRay
{
public:
	DOT_RecievedRay();
	~DOT_RecievedRay();

private:
	Vector::Vector3 m_lastSeen;
	float m_energy;
	int m_timeDelta;
	std::uint8_t m_emitterID;
};

#endif // !DTO_RECIEVEDRAY_H
