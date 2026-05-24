#include "Material.h"

using namespace ef;

Material::Material(){};

Material::Material(
	std::string name,
	bool isPassable,
	bool isInstantiable,
	Vector3F energyLoss,
	float scatteringCoefficient,
	Vector3F normal,
	uint8_t topTileId,
	uint8_t sideTileId,
	uint8_t bottomTileId,
	ColorRGBA color,
	float roughness,
	float metalness,
	float specular,
	float transmissionCoefficient)
	:
	name(name),
	isPassable(isPassable),
	isInstantiable(isInstantiable),
	energyLoss(energyLoss),
	scatteringCoefficient(scatteringCoefficient),
	normal(normal),
	topTileId(topTileId),
	sideTileId(sideTileId),
	bottomTileId(bottomTileId),
	color(color),
	roughness(roughness),
	metalness(metalness),
	specular(specular),
	transmissionCoefficient(transmissionCoefficient)
{}

const bool Material::operator==(Material& other) const {
	return name == other.name;
}