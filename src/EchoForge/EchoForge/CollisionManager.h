#pragma once

#include "RayStrategy.h"
#include "Ray.h"
#include <random>

namespace tracing{

	// Manager des strategies (Anciennement DecisionTree)
	class CollisionManager
	{
	public:
		CollisionManager() = default;
		~CollisionManager() = default;
		// scatteringCoefficient [0,1] : 0=lisse(spéculaire), 1=rugueux(diffus)
		// transmissionCoefficient [0,1] : fraction d'énergie qui traverse le mur
		std::unique_ptr<RayStrategy> decision(Ray& ray, float scatteringCoefficient, float transmissionCoefficient, std::mt19937& rng);




	};

}