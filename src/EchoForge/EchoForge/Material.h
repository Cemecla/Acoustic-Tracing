#pragma once

#include <string>
#include "Vector.h"

namespace ef {
	struct Material
	{
		std::string name{};
		bool isPassable{};
		Vector3F scatteringCoefficient{};
		Vector3F normal{};
		Vector3F energyLoss{};
		

		Material();

		Material(std::string name, bool isPassable, Vector3F energyLoss, Vector3F scatteringCoefficient, Vector3F normal);

		const bool operator==(Material& material) const {
			return name == name;
		}
	};
}