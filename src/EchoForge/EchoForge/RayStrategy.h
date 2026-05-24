#pragma once
#include <random>
#include <string>
#include <Eigen/StdVector>
#include <Eigen/Dense>

namespace tracing {

	class RayStrategy
	{
	public:
		virtual ~RayStrategy() = default;
		RayStrategy(std::string StrategyName, bool isTransmission = false);
		virtual Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const = 0;

		std::string getName() const;
		bool isTransmission() const { return m_isTransmission; }
	protected:
		std::string p_name;
		bool m_isTransmission;

	};


	class SpecularRayStrategy : public RayStrategy {
	public:
		SpecularRayStrategy();
		Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const override;
	};

	class DiffuseRayStrategy : public RayStrategy {
	public:
		DiffuseRayStrategy();
		Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const override;
	};


	class TransmissionRayStrategy : public RayStrategy {
	public:
		TransmissionRayStrategy();
		// Le son traverse le mur sans changer de direction (pas de réfraction de Snell).
		// L'atténuation est gérée par energyLoss du matériau dans RayManager::next_colision().
		Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const override;
	};
}
