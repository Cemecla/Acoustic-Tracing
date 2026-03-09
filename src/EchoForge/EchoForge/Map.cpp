#include "Map.h"

Map::Map(): m_width(16), m_height(16), m_depth(16), m_voxelSizeCm(100.0){}

Map::Map(int width, int height, int depth): m_width(width), m_height(height), m_depth(depth), m_voxelSizeCm(100.0) {}

Map::Map(int width, int height, int depth, float voxelSizeCm): m_width(width), m_height(height), m_depth(depth), m_voxelSizeCm(voxelSizeCm) {}

const Materiel Map::fetchMateriel(const int x, const int y, const int z) const {
	int index = findId(x, y, z);
	return static_cast<Materiel>(m_map[index]);
}

const std::uint8_t Map::fetchMaterielId(const int x, const int y, const int z) const {
	int index = findId(x, y, z);
	return m_map[index];
}

void Map::alterMateriel(const int x, const int y, const int z, Materiel materiel) {
	int index = findId(x, y, z);
	m_map[index] = static_cast<int>(materiel);
}

void Map::destroyMateriel(const int x, const int y, const int z) {
	int index = findId(x, y, z);
	m_map[index] = static_cast<int>(Materiel::EMPTY);
}

inline const int Map::findId(const int x, const int y, const int z) const {
	return z * (m_height * m_width) + y * m_width + x; // To test and verify
}

const bool Map::assertOutOfBound(const int x, const int y, const int z) const {
	if (0 > y > m_height || 0 > x > m_width || 0 > z > m_depth)
		return false;

	return true;
}