#pragma once

#include <vector>
#include <chrono>
#include "Vector.h"

#ifndef RAY_H
	#define RAY_H
	namespace Tracing{
	class Ray
	{
		private:
			EF::Vector3 m_startPosition;
			EF::Vector3 m_lastSeen;
			float m_energy;
			std::chrono::high_resolution_clock::time_point m_time ;
			EF::Vector3 m_velocity;
			EF::Vector3 m_currentPosition;

	public:
		// Method : change energy with a absorption value
		// - float  0 - 1 , absorption of materials
		void loseEnergy(const float& absorptin);

		float getEnergy();
		
		// Method: get current position of the ray
		EF::Vector3 getPosition();
		
		//Method: get time (ms) since ray init
		int getTimeDelta();

		// Constructor
		Ray(EF::Vector3 position, EF::Vector3 velocity);


	};

	}
	

#endif // !__Ray__


