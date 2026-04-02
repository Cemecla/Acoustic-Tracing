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
		RayStrategy(std::string StrategyName);
		virtual Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const = 0;

		std::string getName() const;
	protected:
		std::string p_name;

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
		TransmissionRayStrategy(float ior =1.5f);
		Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, std::mt19937& rng) const override;
	
	private:
		float m_indexOfRefraction;
	};
}
