#pragma once

#include <cstdint>
#include <vector>


enum class Materiel: uint8_t {
	EMPTY,
	STONE,
	GLASS,
	WOOD,
	OUT_OF_BOUND
};

#ifndef MAP_H
	#define MAP_H

	class Map {
		// Y is heigh axis, x width, z lenght
		public:
			Map(int width, int height, int length);
			const Materiel fetchMateriel(const int& x, const int& y, const int& z) const;
			const std::uint8_t fetchMaterielId(const int& x, const int& y, const int& z) const;

			void alterMateriel(const int& x, const int& y, const int& z, Materiel materiel);
			void destroyMateriel(const int& x, const int& y, const int& z);
		
		private:
			int m_width;
			int m_height;
			int m_length;
			std::vector<int> m_map;

			const int findId(const int& x, const int& y, const int& z) const;
			const bool assertOutOfBound(const int& x, const int& y, const int& z) const;
	};

#endif