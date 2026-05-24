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
		std::unique_ptr<RayStrategy> decision(Ray& ray, float materialCoefficient, std::mt19937& rng);

	private:
		// Proportion fixe du son qui traverse le mur (indépendante du matériau)
		// Le reste est partagé entre diffus et spéculaire selon materialCoefficient
		const float m_transmission_prob = 0.1f;




	};

}