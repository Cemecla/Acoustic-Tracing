#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "Vector.h"
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
		class Map {
			// Y is height axis, x width, z depth
			public:
				Map();
				Map(unsigned width, unsigned height, unsigned depth);
				Map(unsigned width, unsigned height, unsigned depth, float voxelSizeCm);
	
				const Material& fetchMaterial(const int x, const int y, const int z) const;
				const Material& fetchMaterial(const Vector3I& position) const;
	
				const int fetchMaterialId(const Material& material) const;
				const int fetchMaterialId(const int x, const int y, const int z) const;
				const int fetchMaterialId(const Vector3I& position) const;
	
				void alterVoxelMaterial(const int x, const int y, const int z, int id);
				void alterVoxelMaterial(const Vector3I& position, int id);
	
				void destroyVoxelMaterial(const int x, const int y, const int z);
				void destroyVoxelMaterial(const Vector3I& position);
	
			private:
				unsigned m_width;
				unsigned m_height;
				unsigned m_depth;
				float m_voxelSizeCm;
				std::string name;
				std::vector<Material> m_materials;
				std::vector<int> m_map = {
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
					1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
					1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
				};
	
				inline const int findMapIndex(const int x, const int y, const int z) const;
	
				inline const int findMapIndex(const Vector3I& position) const;
	
				inline const bool isInBound(const int x, const int y, const int z) const;
		};
	}

#endif