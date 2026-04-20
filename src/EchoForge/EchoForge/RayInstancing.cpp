#include "RayInstancing.h"

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

void RayInstancing::setRays() {

}

QByteArray RayInstancing::getInstanceBuffer(int* instanceCount) {
	*instanceCount = m_count;
	return m_buffer;
}
