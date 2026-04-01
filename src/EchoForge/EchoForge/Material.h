#pragma once

#include <string>
#include "Vector.h"

namespace ef {
	struct Material
	{
		std::string name{};
		bool isPassable{};
		Vector3 scatteringCoefficient{};
		Vector3 normal{};
		Vector3 energyLoss{};
		

		Material();

		Material(std::string name, bool isPassable, Vector3 energyLoss, Vector3 scatteringCoefficient, Vector3 normal);

		const bool operator==(Material& material) const {
			return name == name;
		}
	};
}