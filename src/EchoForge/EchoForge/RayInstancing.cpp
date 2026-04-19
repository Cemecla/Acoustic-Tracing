#include "RayInstancing.h"

RayInstancing::RayInstancing(QQuick3DObject* parent)
	: QQuick3DInstancing(parent)
{}

RayInstancing::~RayInstancing()
{}


QByteArray RayInstancing::getInstanceBuffer(int* instanceCount) {
	*instanceCount = m_count;
	return m_buffer;
}
