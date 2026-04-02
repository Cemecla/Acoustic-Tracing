#include "CollisionManager.h"
#include "RayStrategy.h"


using namespace tracing;


std::unique_ptr<RayStrategy> CollisionManager::decision(Ray& ray, float materialCoefficient, std::mt19937& rng) {

    float diffuse_prob = m_diffuse_threshold * materialCoefficient;
    float specular_prob = m_specular_threshold * (1.0f - materialCoefficient);
    float transmission_prob = m_transmission_threshold; 

    float total = diffuse_prob + specular_prob + transmission_prob;
    diffuse_prob /= total;
    specular_prob /= total;

    std::uniform_real_distribution<float> dist(0.f, 1.f);
    float rand = dist(rng);

    if (rand < diffuse_prob)
        return std::make_unique<DiffuseRayStrategy>();
    else if (rand < diffuse_prob + specular_prob)
        return std::make_unique<SpecularRayStrategy>(); 
    else
        return std::make_unique<TransmissionRayStrategy>();

}