#include "ChunkInstancing.h"
#include <qdebug.h>

#include "Vector.h"

// https://doc.qt.io/qt-6/qtquick3d-custominstancing-example.html

ChunkInstancing::ChunkInstancing(QQuick3DObject *parent): QQuick3DInstancing(parent)
{

}

void ChunkInstancing::setChunk(const ef::Map& map) {

	int temp_count = 0;

	int voxelUnitSize = map.getUnitSize();

	int halfVoxelSize = voxelUnitSize / 2;

	QVector3D rotation(0, 0, 0);
	QVector3D scale(1, 1, 1);

	const std::vector<ef::Material>& rawMaterials = map.getRawMaterials();
	const std::vector<int>& voxels = map.getVoxels();

	qDebug() << "start";

	m_buffer.clear();
	m_buffer.reserve(voxels.size() * sizeof(InstanceTableEntry));

	int i = 0;
	int count = 0;
	for (int d = 0; d < map.getDepth(); d++) {

		int df = d * voxelUnitSize + halfVoxelSize;

		for (int h = 0; h < map.getHeight(); h++) {

			int hf = h * voxelUnitSize + halfVoxelSize;

			for (int w = 0; w < map.getWidth(); w++) {

				const ef::Material& material = rawMaterials[voxels[i++]];

				if (!material.isInstantiable)
					continue;

				QVector3D translation(w * voxelUnitSize + halfVoxelSize, hf, df);
				QColor color(material.color.r, material.color.g, material.color.b, material.color.a);

				QVector4D custom = { material.metalness, material.roughness, material.specular, 0.0 };

				auto entry = calculateTableEntry(translation, scale, rotation, color, custom);
				m_buffer.append(reinterpret_cast<const char*>(&entry), sizeof(entry));

				count++;
			}
		}
	}

	m_count = count;
	qDebug() << "end";
	markDirty();
}

QByteArray ChunkInstancing::getInstanceBuffer(int* instanceCount) {
	*instanceCount = m_count;
	return m_buffer;
}
