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
		// Y is height axis, x width, z depth
		public:
			Map();
			Map(int width, int height, int depth);
			Map(int width, int height, int depth, float voxelSizeCm);
			const Materiel fetchMateriel(const int x, const int y, const int z) const;
			const std::uint8_t fetchMaterielId(const int x, const int y, const int z) const;

			void alterMateriel(const int x, const int y, const int z, Materiel materiel);
			void destroyMateriel(const int x, const int y, const int z);
		
		private:
			int m_width;
			int m_height;
			int m_depth;
			float m_voxelSizeCm;
			std::vector<int> m_map = {
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
				1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
				1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
			};

			inline const int findId(const int x, const int y, const int z) const;
			const bool assertOutOfBound(const int x, const int y, const int z) const;
	};

#endif