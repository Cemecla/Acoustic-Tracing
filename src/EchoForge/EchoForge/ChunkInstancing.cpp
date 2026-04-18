#include "ChunkInstancing.h"
#include <qdebug.h>
#include <omp.h>

#include "Vector.h"

ChunkInstancing::ChunkInstancing(QQuick3DObject *parent): QQuick3DInstancing(parent)
{

}


void ChunkInstancing::setChunk(const ef::Map& map) {
	m_buffer.clear();
	int temp_count = 0;

	int voxelUnitSize = map.getUnitSize();

	int halfVoxelSize = voxelUnitSize / 2;

	//#pragma omp parallel for
	//for (int validId : map.getActiveVoxelsId()) {
	//	const ef::Vector3I pos = map.getPositionFromId(validId);
	//	ef::Material material = map.fetchMaterial(pos);


	//	QVector3D translation(pos.x * voxelUnitSize + halfVoxelSize, pos.y * voxelUnitSize + halfVoxelSize, pos.z * voxelUnitSize + halfVoxelSize);
	//	QVector3D scale(0.2, 0.2, 0.2);
	//	QVector3D rotation(0, 0, 0);
	//	QColor color(255, 255, 255, 255);

	//	temp_count++;
	//	auto entry = calculateTableEntry(translation, scale, rotation, color);
	//	m_buffer.append(reinterpret_cast<const char*>(&entry), sizeof(entry));
	//}

	qDebug() << "start";

	for (int d = 0; d < map.getDepth(); d++) {
		for (int h = 0; h < map.getHeight(); h++) {
			for (int w = 0; w < map.getWidth(); w++) {
				ef::Material material = map.fetchMaterial(w, h, d);

				QVector3D translation(w * voxelUnitSize + halfVoxelSize, h * voxelUnitSize + halfVoxelSize, d * voxelUnitSize + halfVoxelSize);
				QVector3D scale(0.2, 0.2, 0.2);
				QVector3D rotation(0, 0, 0);
				QColor color(255, 255, 255, 255);

				temp_count++;
				auto entry = calculateTableEntry(translation, scale, rotation, color);
				m_buffer.append(reinterpret_cast<const char*>(&entry), sizeof(entry));
			}
		}
	}

	m_count = temp_count;
	qDebug() << "end";
	markDirty();
}

QByteArray ChunkInstancing::getInstanceBuffer(int* instanceCount) {
	*instanceCount = m_count;
	return m_buffer;
}
