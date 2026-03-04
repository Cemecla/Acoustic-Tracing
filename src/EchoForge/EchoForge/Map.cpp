#include "Map.h"

Map::Map(int width, int height, int length): m_width(width), m_height(height), m_length(length) {

}

const Materiel Map::fetchMateriel(const int& x, const int& y, const int& z) const {
	int index = findId(x, y, z);
	return static_cast<Materiel>(m_map[index]);
}

const std::uint8_t Map::fetchMaterielId(const int& x, const int& y, const int& z) const {
	int index = findId(x, y, z);
	return m_map[index];
}

void Map::alterMateriel(const int& x, const int& y, const int& z, Materiel materiel) {
	int index = findId(x, y, z);
	m_map[index] = static_cast<int>(materiel);
}

void Map::destroyMateriel(const int& x, const int& y, const int& z) {
	int index = findId(x, y, z);
	m_map[index] = static_cast<int>(Materiel::EMPTY);
}

const int Map::findId(const int& x, const int& y, const int& z) const {
	return z * (m_height * m_width) + y * m_width + x; // To test and verify
}

const bool Map::assertOutOfBound(const int& x, const int& y, const int& z) const {
	if (0 > y > m_height || 0 > x > m_width || 0 > z > m_length)
		return false;

	return true;
}