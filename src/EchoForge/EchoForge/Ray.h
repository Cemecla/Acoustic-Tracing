#pragma once

#include <vector>
#include <chrono>

#ifndef RAY_H
	#define RAY_H
	
	class Ray
	{
		private:
			std::vector<float> m_startPosition;
			std::vector<float> m_lastSeen;
			float m_energy;
			std::chrono::high_resolution_clock::time_point m_time ;
			std::vector<float> m_velocity;
			std::vector<float> m_currentPosition;

	public:
		// Method : change energy with a absorption value
		// - float  0 - 1 , absorption of materials
		void loseEnergy(float absorption);

		float getEnergy();
		
		// Method: get current position of the ray
		std::vector<float> getPosition();
		
		//Method: get time (ms) since ray init
		int getTimeDelta();

		// Constructor
		Ray(std::vector<float> position, std::vector<float> velocity);


	};
	

#endif // !__Ray__


