#include "CollisionManager.h"
#include "RayStrategy.h"
using namespace tracing;




std::unique_ptr<RayStrategy> CollisionManager::decision(Ray& ray, float& materialCoefficient) {
	
	return std::make_unique<SpecularRayStrategy>();
}