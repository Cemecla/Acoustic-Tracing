#include "RayInstancing.h"
#include <QQuaternion>

RayInstancing::RayInstancing(QQuick3DObject* parent)
	: QQuick3DInstancing(parent)
{}

void RayInstancing::clearBuffer() {
	m_buffer.clear();
	m_count = 0;
}

void RayInstancing::prepareBuffer(int batchSize) {
	m_buffer.reserve(batchSize * sizeof(InstanceTableEntry));
}

void RayInstancing::setRays(const std::vector<DTO_RecievedRay>& rays) {
	clearBuffer();

	int rayCount = 0;
	for (const auto& ray : rays)
		rayCount += static_cast<int>(ray.getBouncePoints().size());

	if (rayCount == 0) {
		markDirty();
		return;
	}

	prepareBuffer(rayCount);

	for (const auto& ray : rays)
	{
		const auto& points = ray.getBouncePoints();
		const auto& lastPoint = ray.getLastSeen();

		for (size_t i = 0; i < points.size(); ++i)
		{
			const Eigen::Vector3f& s = points[i];
			const Eigen::Vector3f& e = (i + 1 < points.size()) ? points[i + 1] : lastPoint;

			InstanceTableEntry entry = makeEntry( { s.x(), s.y(), s.z() }, { e.x(), e.y(), e.z() } );

			m_buffer.append(reinterpret_cast<const char*>(&entry), sizeof(entry));
		}
	}

	m_count = rayCount;
	markDirty();
}

RayInstancing::InstanceTableEntry RayInstancing::makeEntry(const QVector3D& start, const QVector3D& end)
{
	QVector3D dir = end - start;
	float length = dir.length();

	QVector3D position = (start + end) * 0.5f;

	const float thickness = 0.05f;
	QVector3D scale(thickness, length, thickness);

	QQuaternion rotation;
	if (length > 0) {
		rotation = QQuaternion::rotationTo(QVector3D(0, 1, 0), dir / length);
	}

	return calculateTableEntryFromQuaternion(
		position,
		scale,
		rotation,
		QColor(255, 0, 0, 255),
		QVector4D()
	);
}

QByteArray RayInstancing::getInstanceBuffer(int* instanceCount) {
	*instanceCount = m_count;
	return m_buffer;
}
