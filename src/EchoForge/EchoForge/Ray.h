#pragma once

#include <vector>
#include <chrono>
//#include "Vector.h"
#include <Eigen/Dense>

#ifndef RAY_H
	#define RAY_H
	namespace tracing{
	class Ray
	{
		private:
			Eigen::Vector3f m_startPosition;
			Eigen::Vector3f m_lastSeen;
			float m_energy;
			std::chrono::high_resolution_clock::time_point m_time ;
			Eigen::Vector3f m_velocity;
			Eigen::Vector3f m_currentPosition;
			void m_next();
			Eigen::Vector3i m_voxelPos;
			Eigen::Vector3i m_step;
			Eigen::Vector3i m_tDelta;
			Eigen::Vector3f m_tMax;


	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
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
		Eigen::Vector3f getPosition();
		
		/**
		*	desc: This method returns time delay since ray was launched in ms
		*/
		int getTimeDelta();

		// Constructor
		Ray(Eigen::Vector3f position, Eigen::Vector3f velocity);


	};

	}
	

#endif // !__Ray__


