#include "CollisionManager.h"
#include "RayStrategy.h"


using namespace tracing;


std::unique_ptr<RayStrategy> CollisionManager::decision(Ray& ray, float materialCoefficient, std::mt19937& rng) {
    // materialCoefficient (scattering) ∈ [0,1] : 0 = lisse, 1 = rugueux
    // Les 3 probabilités somment à 1 par construction
    float diffuse_prob      = materialCoefficient * (1.0f - m_transmission_prob);
    float specular_prob     = (1.0f - materialCoefficient) * (1.0f - m_transmission_prob);
    // m_transmission_prob est fixe : proportion du son qui traverse le mur

    std::uniform_real_distribution<float> dist(0.f, 1.f);
    float rand = dist(rng);

    if (rand < diffuse_prob)
        return std::make_unique<DiffuseRayStrategy>();
    else if (rand < diffuse_prob + specular_prob)
        return std::make_unique<SpecularRayStrategy>();
    else
        return std::make_unique<TransmissionRayStrategy>();
}