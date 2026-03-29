#pragma once
#include "Vector.h"
#include "Queue.h"
#include "Ray.h"
#include <Eigen/StdVector>

#ifndef RAYMANAGER_H
	#define RAYMANAGER_H


class EFModel;

namespace tracing {


	class RayManager
	{
	public:
		RayManager(EFModel* parent,int rayCount);
		const void update();
	private:
		constexpr static float PHI = (1.0f + 2.2360679775f) / 2.0f;
		constexpr static float k_absorption[5] = {0.001,0.97f,0.20f,0.65f,1.0f}; // Temporary
		int m_rayCount;
		const void createRays(Eigen::Vector3f position);
		const void next_colision();
		std::unique_ptr<std::vector<Ray>> m_activeRays = std::make_unique<std::vector<Ray>>();
		EFModel* m_parent;
	

	};

};
#endif // !RAYMANAGER_H
