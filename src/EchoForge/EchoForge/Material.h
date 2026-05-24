#pragma once

#include <string>
#include "Vector.h"
#include "Utils.h"

namespace ef {
	struct Material
	{
		std::string name{};
		bool isPassable{};
		bool isInstantiable{};
		float scatteringCoefficient{};
		Vector3F normal{};
		Vector3F energyLoss{};
		uint8_t topTileId{};
		uint8_t sideTileId{};
		uint8_t bottomTileId{};
		ColorRGBA color;
		float roughness{};
		float metalness{};
		float specular{};
		// Fraction [0,1] de l'énergie sonore qui traverse le matériau (transmission acoustique).
		// 0 = opaque au son, 1 = totalement transparent. Physiquement : béton ~0.03, verre ~0.40.
		float transmissionCoefficient{};

		Material();

		Material(
			std::string name,
			bool isPassable,
			bool isInstantiable,
			Vector3F energyLoss,
			float scatteringCoefficient,
			Vector3F normal,
			uint8_t topTileId,
			uint8_t sideTileId,
			uint8_t bottomTileId,
			ColorRGBA color = ColorRGBA{},
			float roughness = 0,
			float metalness = 0,
			float specular = 0,
			float transmissionCoefficient = 0.0f
		);

		const bool operator==(Material& other) const;
	};
}