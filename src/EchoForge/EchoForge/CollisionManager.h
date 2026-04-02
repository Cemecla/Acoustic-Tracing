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
		//TODO Refaire les threshold
		const float m_diffuse_threshold = 0.30;
		const float m_specular_threshold = 0.30;
		const float m_transmission_threshold = 0.30;




	};

}