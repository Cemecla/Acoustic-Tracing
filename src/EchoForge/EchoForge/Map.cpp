#include "Map.h"
#include "Material.h"

using namespace ef;

Map::Map(): m_width(16), m_height(16), m_depth(16), m_voxelSizeCm(100.0) {}

Map::Map(unsigned width, unsigned height, unsigned depth): m_width(width), m_height(height), m_depth(depth), m_voxelSizeCm(100.0) {}

Map::Map(unsigned width, unsigned height, unsigned depth, float voxelSizeCm): m_width(width), m_height(height), m_depth(depth), m_voxelSizeCm(voxelSizeCm) {}

const Material& Map::fetchMaterial(const int x, const int y, const int z) const {
	int mapIndex = findMapIndex(x, y, z);

	int materialIndex = m_map[mapIndex];

	return m_materials[materialIndex];
}

const Material& Map::fetchMaterial(const Vector3I& position) const {
	int mapIndex = findMapIndex(position);

	int materialIndex = m_map[mapIndex];

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
	return m_map[mapIndex];
}

const int Map::fetchMaterialId(const Vector3I& position) const {
	int mapIndex = findMapIndex(position);
	return m_map[mapIndex];
}

void Map::alterVoxelMaterial(const int x, const int y, const int z, int id) {

}

void Map::alterVoxelMaterial(const Vector3I& position, int id) {

}

void Map::destroyVoxelMaterial(const int x, const int y, const int z) {

}

void Map::destroyVoxelMaterial(const Vector3I& position) {

}

inline const int Map::findMapIndex(const int x, const int y, const int z) const {
	return z * (m_height * m_width) + y * m_width + x; // To test and verify
}

inline const int Map::findMapIndex(const Vector3I& position) const {
	return position.z * (m_height * m_width) + position.y * m_width + position.x;
}

// https://lemire.me/blog/2025/03/24/unsigned-comparisons-using-signed-types/
inline const bool Map::isInBound(const int x, const int y, const int z) const {
	return (unsigned)x < m_width && (unsigned)y < m_height && (unsigned)z < m_depth;
}