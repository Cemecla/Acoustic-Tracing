#pragma once

#include <vector>
#include "Vector.h"
#include <Eigen/Dense>

#ifndef RAY_H
	#define RAY_H
	namespace tracing{
	class Ray
	{
		private:
			Eigen::Vector3f m_spawnPosition;
			Eigen::Vector3f m_currentPosition;
			Eigen::Vector3f m_startPosition;
			Eigen::Vector3f m_lastSeen;
			Eigen::Vector3f m_energy;
			int             m_stepCount = 0;
			Eigen::Vector3f m_velocity;
			Eigen::Vector3i m_voxelPos;
			Eigen::Vector3i m_step;
			Eigen::Vector3f m_tDelta;
			Eigen::Vector3f m_tMax;

			int  m_lastAxis;
		bool m_wasInSolid = false;
		std::vector<Eigen::Vector3f> m_bouncePositions;

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		/**
		*	desc: This method removes energy to the ray depending on a material coefficient
		*   param mods: float between 0 and 1.
		*/
		void loseEnergy(const Eigen::Vector3f& absorption);

		void advance();

		const Eigen::Vector3f& getEnergy() const;

		Eigen::Vector3i getVoxelPos() const;

		Eigen::Vector3f getVelocity() const;

		void redirect(const Eigen::Vector3f& newVelocity);

		Eigen::Vector3f getHitNormal() const;

		bool wasInSolid() const { return m_wasInSolid; }
		void setInSolid(bool value) { m_wasInSolid = value; }

		[[nodiscard]] const std::vector<Eigen::Vector3f>& getBouncePositions() const { return m_bouncePositions; }

		
		/**
		*	desc: This method returns a position (x,y,z)
		*/
		Eigen::Vector3f getPosition();
		
		int getStepCount() const { return m_stepCount; }

		// Constructor
		Ray(Eigen::Vector3f position, Eigen::Vector3f velocity, int unitSize);


	};

	}
	

#endif // !__Ray__


