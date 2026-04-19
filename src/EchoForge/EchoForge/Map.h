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
				Map(unsigned width, unsigned height, unsigned depth, int voxelUnitValueCm, int voxelUnitSize, std::vector<int> voxels, std::vector<Material> materials);
	
				const Material& fetchMaterial(const float x, const float y, const float z) const;
				const Material& fetchMaterial(const int x, const int y, const int z) const;
				const Material& fetchMaterial(const Vector3F& worldPosition) const;
				const Material& fetchMaterial(const Vector3I& position) const;
	
				const int fetchMaterialId(const float x, const float y, const float z) const;
				const int fetchMaterialId(const Vector3F& worldPosition) const;
				const int fetchMaterialId(const Material& material) const;
				const int fetchMaterialId(const int x, const int y, const int z) const;
				const int fetchMaterialId(const Vector3I& position) const;
	
				void alterVoxelMaterial(const int x, const int y, const int z, int id);
				void alterVoxelMaterial(const Vector3I& position, int id);
	
				void destroyVoxelMaterial(const int x, const int y, const int z);
				void destroyVoxelMaterial(const Vector3I& position);

				Vector3I getPositionFromId(int index) const;

				const unsigned getWidth() const;
				const unsigned getHeight() const;
				const unsigned getDepth() const;
				const int countVoxels() const;
				const int getUnitSize() const;
				const int getUnitValue() const;
				const std::vector<int>& getActiveVoxelsId() const;
				const std::vector<Material>& getRawMaterials() const;
				const std::vector<int>& getVoxels() const;
				const std::string getName() const;

				// https://lemire.me/blog/2025/03/24/unsigned-comparisons-using-signed-types/
				const bool isInBound(const int x, const int y, const int z) const {
					return (unsigned)x < m_width && (unsigned)y < m_height && (unsigned)z < m_depth;
				}

	
			private:
				unsigned m_width;
				unsigned m_height;
				unsigned m_depth;
				int m_voxelUnitSize;
				int m_voxelUnitValueCm;
				std::string m_name;
				std::vector<Material> m_materials;
				std::vector<int> m_voxels;
				std::vector<int> m_activeVoxelsId;
	

				const int findWorldPositionIndex(const Vector3F& worldPosition) const;

				const int findWorldPositionIndex(const float x, const float y, const float z) const;

				const int findMapIndex(const int x, const int y, const int z) const;
	
				const int findMapIndex(const Vector3I& position) const;

				void buildEmptyMap();
		};
	}

#endif