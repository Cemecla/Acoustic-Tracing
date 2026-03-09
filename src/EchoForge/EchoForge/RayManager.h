#pragma once
#include "Vector.h"
#include "Queue.h"
#include "Ray.h"

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
		const void createRays(ef::Vector3 position);
		ef::Queue<std::vector<Ray>> * m_rayQueue;
	};

};
#endif // !RAYMANAGER_H
