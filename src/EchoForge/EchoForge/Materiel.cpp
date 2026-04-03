#include "Material.h"

ef::Material::Material(){};

ef::Material::Material(std::string name, bool isPassable, ef::Vector3F energyLoss, float scatteringCoefficient, ef::Vector3F normal): name(name), isPassable(isPassable), energyLoss(energyLoss), scatteringCoefficient(scatteringCoefficient), normal(normal) {};