#pragma once
#include "Vector.h"
#include "Queue.h"
#include "Ray.h"
#include <Eigen/StdVector>

#ifndef RAYMANAGER_H
	#define RAYMANAGER_H

namespace tracing {


	class RayManager
	{
	public:
		RayManager(int rayCount);
		~RayManager();
		const void update();
	private:
		constexpr static float PHI = (1.0f + 2.2360679775f) / 2.0f;
		int m_rayCount;
		const void createRays(Eigen::Vector3f position);
		const void next_colision();
		std::unique_ptr<std::vector<Ray, Eigen::aligned_allocator<Ray>>> m_activeRays;
	

	};

};
#endif // !RAYMANAGER_H
