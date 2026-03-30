#include "RayStrategy.h"

namespace tracing{

	RayStrategy::RayStrategy(std::string StrategyName) : p_name(StrategyName) {};

	std::string RayStrategy::getName() const {
		return p_name;
	}



	SpecularRayStrategy::SpecularRayStrategy(): RayStrategy("Specular") {}
	Eigen::Vector3f SpecularRayStrategy::getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, float& rng) const{
		return inDir - 2.0f *inDir.dot(normal) * normal;
	}

}