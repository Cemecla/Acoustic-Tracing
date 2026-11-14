#include <cassert>

#include "Map.h"
#include "Material.h"

using namespace ef;
using namespace std;

Map::Map(): m_width(16), m_height(16), m_depth(16), m_voxelUnitValueCm(100), m_voxelUnitSize(100), m_voxels({}), m_materials({}) {
	buildEmptyMap();
}

Map::Map(
	unsigned width, 
	unsigned height, 
	unsigned depth
): 
	m_width(width), 
	m_height(height), 
	m_depth(depth), 
	m_voxelUnitValueCm(100), 
	m_voxelUnitSize(100), 
	m_voxels({}),
	m_materials({}) 
{ buildEmptyMap(); }

Map::Map(
	unsigned width, 
	unsigned height, 
	unsigned depth, 
	int voxelUnitValueCm, 
	int voxelUnitSize, 
	std::vector<int> voxels, 
	std::vector<Material> materials
): 
	m_width(width),
	m_height(height),
	m_depth(depth),
	m_voxelUnitValueCm(voxelUnitValueCm),
	m_voxelUnitSize(voxelUnitSize),
	m_voxels(voxels),
	m_materials(materials) 
{}

const Material& Map::fetchMaterial(const int x, const int y, const int z) const {
	int mapIndex = findMapIndex(x, y, z);

	assert(mapIndex >= 0 && mapIndex < m_voxels.size());
	int materialIndex = m_voxels[mapIndex];

	return m_materials[materialIndex];
}

const Material& Map::fetchMaterial(const Vector3I& position) const {
	int mapIndex = findMapIndex(position);

	int materialIndex = m_voxels[mapIndex];

	return m_materials[materialIndex];
}

const int Map::fetchMaterialId(const Material& material) const {
	for (int i = 0; i < m_materials.size(); i++)
	{
		Material current = m_materials[i];
		if (material == current)
			return i;
	}

	return -1;
}

const int Map::fetchMaterialId(const int x, const int y, const int z) const {
	int mapIndex = findMapIndex(x, y, z);
	return m_voxels[mapIndex];
}

const int Map::fetchMaterialId(const Vector3I& position) const {
	int mapIndex = findMapIndex(position);
	return m_voxels[mapIndex];
}

void Map::alterVoxelMaterial(const int x, const int y, const int z, int id) {

}

void Map::alterVoxelMaterial(const Vector3I& position, int id) {

}

void Map::destroyVoxelMaterial(const int x, const int y, const int z) {

}

void Map::destroyVoxelMaterial(const Vector3I& position) {

}

const int Map::findMapIndex(const int x, const int y, const int z) const {
	return z * (m_height * m_width) + y * m_width + x; // To test and verify
}

const int Map::findMapIndex(const Vector3I& position) const {
	return position.z * (m_height * m_width) + position.y * m_width + position.x;
}

const int Map::findWorldPositionIndex(const Vector3F& worldPosition) const {
	int nx = (int)floor(worldPosition.x / m_voxelUnitSize);
	int ny = (int)floor(worldPosition.y / m_voxelUnitSize);
	int nz = (int)floor(worldPosition.z / m_voxelUnitSize);

	return findMapIndex(nx, ny, nz);
}

const int Map::findWorldPositionIndex(const float x, const float y, const float z) const {
	int nx = (int)floor(x / m_voxelUnitSize);
	int ny = (int)floor(y / m_voxelUnitSize);
	int nz = (int)floor(z / m_voxelUnitSize);

	return findMapIndex(nx, ny, nz);
}

const Material& Map::fetchMaterial(const Vector3F& worldPosition) const {
	int mapIndex = findWorldPositionIndex(worldPosition);

	int materialIndex = m_voxels[mapIndex];

	return m_materials[materialIndex];
}

const Material& Map::fetchMaterial(const float x, const float y, const float z) const {
	int mapIndex = findWorldPositionIndex(x, y, z);

	int materialIndex = m_voxels[mapIndex];

	return m_materials[materialIndex];
}

const int Map::fetchMaterialId(const float x, const float y, const float z) const {
	int mapIndex = findWorldPositionIndex(x, y, z);
	return m_voxels[mapIndex];
}

const int Map::fetchMaterialId(const Vector3F& worldPosition) const {
	int mapIndex = findWorldPositionIndex(worldPosition);
	return m_voxels[mapIndex];
}

Vector3I Map::getPositionFromId(int index) const {
	int x = index % m_width;
	int y = (int)std::floor((index / m_width) % m_height);
	int z = (int)std::floor(index / (m_width * m_height));
	return Vector3I(x, y, z);
}

const std::vector<int>& Map::getActiveVoxelsId() const { return m_activeVoxelsId; }

const bool Map::isInBound(const int x, const int y, const int z) const {
	return (unsigned)x < m_width && (unsigned)y < m_height && (unsigned)z < m_depth;
}

void Map::buildEmptyMap() {
	m_voxels.clear();
	int voxelsTotal = m_width * m_height * m_depth;
	for (int i = 0; i < voxelsTotal; i++)
		m_voxels.push_back(1);
}

const unsigned Map::getWidth() const { return m_width; }

const unsigned Map::getHeight() const { return m_height; }

const unsigned Map::getDepth() const { return m_depth; }

const int Map::countVoxels() const { return m_voxels.size(); }

const int Map::getUnitSize() const { return m_voxelUnitSize; }

const int Map::getUnitValue() const { return m_voxelUnitValueCm; }

const std::string Map::getName() const { return m_name; }

const std::vector<int>& Map::getVoxels() const { return m_voxels; }

const std::vector<Material>& Map::getRawMaterials() const { return m_materials; }