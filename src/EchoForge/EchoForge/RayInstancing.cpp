#include "RayInstancing.h"
#include <QQuaternion>

RayInstancing::RayInstancing(QQuick3DObject* parent)
	: QQuick3DInstancing(parent)
{}

void RayInstancing::clearBuffer() {
	m_buffer.clear();
	m_count = 0;
}

void RayInstancing::prepareBuffer(QByteArray& buffer, int batchSize) {
	buffer.reserve(batchSize * sizeof(InstanceTableEntry));
}

void RayInstancing::setRays(const std::vector<DTO_RecievedRay>& rays) {

	int rayCount = 0;
	for (const auto& ray : rays)
		rayCount += static_cast<int>(ray.getBouncePoints().size());

	if (rayCount == 0) {
		clearBuffer();
		m_count = rayCount;
		markDirty();
		return;
	}

	QByteArray tempBuffer;

	prepareBuffer(tempBuffer, rayCount);

	for (const auto& ray : rays)
	{
		const auto& points = ray.getBouncePoints();
		const auto& lastPoint = ray.getLastSeen();

		for (size_t i = 0; i < points.size(); ++i)
		{
			const Eigen::Vector3f& s = points[i];
			const Eigen::Vector3f& e = (i + 1 < points.size()) ? points[i + 1] : lastPoint;

			InstanceTableEntry entry = makeEntry( { s.x(), s.y(), s.z() }, { e.x(), e.y(), e.z() } );

			tempBuffer.append(reinterpret_cast<const char*>(&entry), sizeof(entry));
		}
	}

	clearBuffer();
	m_count = rayCount;
	m_buffer = tempBuffer;
	
	markDirty();
}

RayInstancing::InstanceTableEntry RayInstancing::makeEntry(const QVector3D& start, const QVector3D& end)
{
	QVector3D dir = end - start;
	const float length = dir.length();

	const QVector3D position = (start + end) * 0.5f;

	const QVector3D scale(0.05f, length / 100.0f, 0.05f);

	QQuaternion rotation;
	if (length > 0) {
		const QVector3D dirN = dir / length;
		if (dirN.y() < -0.9999f) {
			rotation = QQuaternion::fromAxisAndAngle(1, 0, 0, 180);
		}
		else {
			rotation = QQuaternion::rotationTo(QVector3D(0, 1, 0), dirN);
		}
	}

	return calculateTableEntryFromQuaternion(
		position, scale, rotation,
		QColor(255, 0, 0, 100), QVector4D());
}

QByteArray RayInstancing::getInstanceBuffer(int* instanceCount) {
	*instanceCount = m_count;
	return m_buffer;
}
