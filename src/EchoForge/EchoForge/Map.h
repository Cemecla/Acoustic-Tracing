#pragma once

#include <cstdint>
#include <vector>
#include "Material.h"


//enum class Materiel: uint8_t {
//	EMPTY,
//	STONE,
//	GLASS,
//	WOOD,
//	OUT_OF_BOUND
//};

#ifndef MAP_H
	#define MAP_H

	namespace ef {
	
		struct Vector3;
	
		class Map {
			// Y is height axis, x width, z depth
			public:
				Map();
				Map(int width, int height, int depth);
				Map(int width, int height, int depth, float voxelSizeCm);
	
				const Material& fetchMaterial(const int x, const int y, const int z) const;
				const Material& fetchMaterial(const Vector3& position) const;
	
				const int fetchMaterialId(const Material& material) const;
				const int fetchMaterialId(const int x, const int y, const int z) const;
				const int fetchMaterialId(const Vector3& position) const;
	
				void alterVoxelMaterial(const int x, const int y, const int z, int id);
				void alterVoxelMaterial(const Vector3& position, int id);
	
				void destroyVoxelMaterial(const int x, const int y, const int z);
				void destroyVoxelMaterial(const Vector3& position);
	
			private:
				int m_width;
				int m_height;
				int m_depth;
				float m_voxelSizeCm;
				std::vector<Material> m_materials;
				std::vector<int> m_map = {
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
					1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
					1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
				};
	
				inline const int findMapIndex(const int x, const int y, const int z) const;
	
				inline const int findMapIndex(const Vector3& position) const;
	
				const bool assertOutOfBound(const int x, const int y, const int z) const;
		};
	
	
	}

#endif