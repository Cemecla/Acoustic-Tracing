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
		ColorRGBA color;
		float roughness{};
		float metalness{};
		float specular{};

		Material();

		Material(std::string name, bool isPassable, bool isInstantiable, Vector3F energyLoss, float scatteringCoefficient, Vector3F normal, ColorRGBA color = ColorRGBA{}, float roughness = 0, float metalness = 0, float specular = 0);

		const bool operator==(Material& other) const {
			return name == other.name;
		}
	};
}