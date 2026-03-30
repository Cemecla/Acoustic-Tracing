#pragma once
#include <string>
#include <Eigen/StdVector>


namespace tracing {

	class RayStrategy
	{
	public:
		virtual ~RayStrategy() = default;
		RayStrategy(std::string StrategyName);
		virtual Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, float& rng) const = 0;

		std::string getName() const;
	protected:
		std::string p_name;
	};


	class SpecularRayStrategy : public RayStrategy {
	public:
		SpecularRayStrategy();
		Eigen::Vector3f getNextDirection(const Eigen::Vector3f& inDir, const Eigen::Vector3f& normal, float& rng) const override;
	};
}
