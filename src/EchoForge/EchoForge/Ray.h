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
			ef::Vector3 m_startPosition;
			ef::Vector3 m_lastSeen;
			float m_energy;
			std::chrono::high_resolution_clock::time_point m_time ;
			ef::Vector3 m_velocity;
			ef::Vector3 m_currentPosition;

	public:
		// Method : change energy with a absorption value
		// - float  0 - 1 , absorption of materials
		void loseEnergy(const float& absorptin);

		float getEnergy();
		
		// Method: get current position of the ray
		ef::Vector3 getPosition();
		
		//Method: get time (ms) since ray init
		int getTimeDelta();

		// Constructor
		Ray(ef::Vector3 position, ef::Vector3 velocity);


	};

	}
	

#endif // !__Ray__


