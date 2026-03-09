#pragma once

#include <vector>
#include <chrono>
#include "Vector.h"

#ifndef RAY_H
	#define RAY_H
	namespace tracing{
	class Ray
	{
		private:
			ef::Vector3 m_startPosition;
			ef::Vector3 m_lastSeen;
			float m_energy;
			std::chrono::high_resolution_clock::time_point m_time ;
			ef::Vector3 m_velocity;
			ef::Vector3 m_currentPosition;
			void m_next();


	public:
		/**
		*	desc: This method removes energy to the ray depending on a material coefficient
		*   param mods: float between 0 and 1.
		*/
		void loseEnergy(const float& absorptin);

		float getEnergy();

		
		/**
		*	desc: This method returns a position (x,y,z)
		* 
		*/
		ef::Vector3 getPosition();
		
		/**
		*	desc: This method returns time delay since ray was launched in ms
		*/
		int getTimeDelta();

		// Constructor
		Ray(ef::Vector3 position, ef::Vector3 velocity);


	};

	}
	

#endif // !__Ray__


