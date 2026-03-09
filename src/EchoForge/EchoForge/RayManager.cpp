#define _USE_MATH_DEFINES
#include "RayManager.h"
#include <cmath>


using namespace Tracing;


RayManager::RayManager(int rayCount) : m_rayCount(rayCount), m_rayQueue{ new ef::Queue<std::vector<Ray>>() } {

}

const void RayManager::update() {
	RayManager::createRays(ef::Vector3(0.0f,0.0f,0.0f));
}



const void RayManager::createRays(ef::Vector3 position) {
	float incrementation = M_PI * 2.0f * PHI;
	std::vector<Ray> rayList = {};


	// From https://devforum.roblox.com/t/generating-equidistant-points-on-a-sphere/874144
	for (int n = 0; n < m_rayCount; n++) {
		float distance = (float)n / m_rayCount;
		float azimuth = incrementation * n;

		float incline = std::acos(1 - 2 * distance);
		
		float multiplier = 10.0f;

		float x = std::sin(incline) * std::cos(azimuth) * multiplier;
		float y = std::sin(incline) * std::sin(azimuth) * multiplier;
		float z = std::cos(incline) * multiplier;

		ef::Vector3 ray_start_absolute = ef::Vector3(x,y,z);
		ef::Vector3 norm = ray_start_absolute.normalize();

		Ray r = Ray(position+ ray_start_absolute, norm);
		rayList.push_back(r);

	}

	m_rayQueue->push(rayList);

}

