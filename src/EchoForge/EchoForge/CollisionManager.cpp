#include "CollisionManager.h"
#include "RayStrategy.h"


using namespace tracing;


std::unique_ptr<RayStrategy> CollisionManager::decision(Ray& ray, float scatteringCoefficient, float transmissionCoefficient, std::mt19937& rng) {
    // scatteringCoefficient ∈ [0,1] : 0 = lisse (spéculaire), 1 = rugueux (diffus)
    // transmissionCoefficient ∈ [0,1] : fraction d'énergie qui traverse le mur
    // Les 3 probabilités somment à 1 par construction.
    float reflected         = 1.0f - transmissionCoefficient;
    float diffuse_prob      = scatteringCoefficient * reflected;
    float specular_prob     = (1.0f - scatteringCoefficient) * reflected;
    // transmissionCoefficient = probabilité de traverser le mur

    std::uniform_real_distribution<float> dist(0.f, 1.f);
    float rand = dist(rng);

    if (rand < diffuse_prob)
        return std::make_unique<DiffuseRayStrategy>();
    else if (rand < diffuse_prob + specular_prob)
        return std::make_unique<SpecularRayStrategy>();
    else
        return std::make_unique<TransmissionRayStrategy>();
}