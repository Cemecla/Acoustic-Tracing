#include "ChunkInstancing.h"

ChunkInstancing::ChunkInstancing(QQuick3DObject *parent): QQuick3DInstancing(parent)
{}


void ChunkInstancing::setChunk(const ef::Map& map) {
	m_buffer.clear();
	int temp_count = 0;

	int voxelUnitSize = map.getUnitSize();

	int halfVoxelSize = voxelUnitSize / 2;

	for (int d = 0; d < map.getDepth(); d++) {
		for (int h = 0; h < map.getHeight(); h++) {
			for (int w = 0; w < map.getWidth(); w++) {
				ef::Material material = map.fetchMaterial(w, h, d);

				if (!material.isInstantiable)
					continue;

				QVector3D translation(w * voxelUnitSize + halfVoxelSize, h * voxelUnitSize + halfVoxelSize, d * voxelUnitSize + halfVoxelSize);
				QVector3D scale(1, 1, 1);
				QVector3D rotation(0, 0, 0);
				QColor color(material.color.r, material.color.g, material.color.b, material.color.a);

				temp_count++;
				auto entry = calculateTableEntry(translation, scale, rotation, color);
				m_buffer.append(reinterpret_cast<const char*>(&entry), sizeof(entry));
			}
		}
	}
	m_count = temp_count;
	markDirty();
}

QByteArray ChunkInstancing::getInstanceBuffer(int* instanceCount) {
	*instanceCount = m_count;
	return m_buffer;
}
