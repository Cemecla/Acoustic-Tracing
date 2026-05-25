#pragma once
#include <random>
#include <numbers>
#include <ranges>
#include <Eigen/StdVector>

#include "Vector.h"
#include "Queue.h"
#include "Ray.h"
#include "CollisionManager.h"
#include "DTO_RecievedRay.h"
#include "AmbientEntry.h"

#ifndef RAYMANAGER_H
	#define RAYMANAGER_H


class EFModel;

namespace tracing {

	class RayManager
	{
	public:
		static constexpr std::uint8_t AMBIENT_EMITTER_ID = 255;

		RayManager(EFModel* parent, int rayCount, int raysPerFrame = 10);
		const void update();

		int getRaysPerFrame() const;
		void setRaysPerFrame(const int raysPerFrame) const;

		[[nodiscard]] const std::vector<DTO_RecievedRay>& getReceivedRays() const;
		void clearReceivedRays();
		void clearAllRays();

		[[nodiscard]] std::vector<std::vector<ef::Vector3F>> getRayPaths() const;

	private:
		int m_rayCount;
		int m_raysPerFrame;
		int m_nextRayIndex = 0;
		const void createRays(Eigen::Vector3f position, int count);
		const void next_colision();
		std::unique_ptr<std::vector<Ray>> m_activeRays = std::make_unique<std::vector<Ray>>();
		std::vector<DTO_RecievedRay> m_receivedRays;
		EFModel* m_parent;
		const bool hasLineOfSight(const Eigen::Vector3f& from, const Eigen::Vector3f& to);

		// Vérifie LOS vers tous les emitters actifs depuis un point de contact et génère les DTOs.
		// Retourne le nombre de LOS checks consommés dans le budget.
		int checkLosToEmitters(const Ray& r, int& budgetRemaining, bool isDirect);

		std::mt19937 m_rng{ std::random_device{}() };
		CollisionManager m_collisionManager;

	};

};
#endif // !RAYMANAGER_H
