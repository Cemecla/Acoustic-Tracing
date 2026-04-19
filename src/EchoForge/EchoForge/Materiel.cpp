#include "Material.h"

ef::Material::Material(){};

ef::Material::Material(std::string name, bool isPassable, bool isInstantiable, ef::Vector3F energyLoss, float scatteringCoefficient, ef::Vector3F normal, ColorRGBA color, float roughness, float metalness, float specular) : name(name), isPassable(isPassable), isInstantiable(isInstantiable), energyLoss(energyLoss), scatteringCoefficient(scatteringCoefficient), normal(normal), color(color), roughness(roughness), metalness(metalness), specular(specular) {

}