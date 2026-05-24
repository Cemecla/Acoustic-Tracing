#include "RayStrategy.h"

namespace tracing{

	RayStrategy::RayStrategy(std::string StrategyName, bool isTransmission) : p_name(StrategyName), m_isTransmission(isTransmission) {};

	std::string RayStrategy::getName() const {
		return p_name;
	}



	SpecularRayStrategy::SpecularRayStrategy(): RayStrategy("Specular") {}
	Eigen::Vector3f SpecularRayStrategy::getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const{
		return inDir - 2.0f *inDir.dot(normal) * normal;
	}


    DiffuseRayStrategy::DiffuseRayStrategy() : RayStrategy("Diffuse") {}

    Eigen::Vector3f DiffuseRayStrategy::getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const {
        std::uniform_real_distribution<float> dist(0.f, 1.f);
        float u1 = dist(rng);
        float u2 = dist(rng);

        float phi = 2.0f * 3.14159265358979323846f * u1;
        float cosTheta = std::sqrt(u2);
        float sinTheta = std::sqrt(1.0f - u2);

        // Base orthonormale autour de la normale (Gram-Schmidt)
        // https://keepmath.com/procede-dorthonormalisation-de-gram-schmidt/
        Eigen::Vector3f up = std::abs(normal.x()) < 0.9f ? Eigen::Vector3f::UnitX() : Eigen::Vector3f::UnitY();
        Eigen::Vector3f tangent = up.cross(normal).normalized();
        Eigen::Vector3f bitangent = normal.cross(tangent);

        return (sinTheta * std::cos(phi) * tangent
            + sinTheta * std::sin(phi) * bitangent
            + cosTheta * normal);
    }


    TransmissionRayStrategy::TransmissionRayStrategy() : RayStrategy("Transmission", true) {}

    // Le son, contrairement à la lumière, ne réfracte pas de façon significative en traversant
    // un mur (la longueur d'onde est grande devant l'épaisseur du mur).
    // La direction est conservée; l'atténuation est gérée par energyLoss dans next_colision().
    Eigen::Vector3f TransmissionRayStrategy::getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const {
        return inDir; // direction inchangée - le son passe tout droit
    }
    

}