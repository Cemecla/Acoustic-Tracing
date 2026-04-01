#include "Material.h"

ef::Material::Material(){};

ef::Material::Material(std::string name, bool isPassable, ef::Vector3 energyLoss, ef::Vector3 scatteringCoefficient, ef::Vector3 normal): name(name), isPassable(isPassable), energyLoss(energyLoss), scatteringCoefficient(scatteringCoefficient), normal(normal) {};