#pragma once
#include "DTO_RecievedRay.h"

#include <QtQuick3D/QQuick3DInstancing>
#include <QtQuick3D/QQuick3DObject>

class RayInstancing : public QQuick3DInstancing
{
	Q_OBJECT
	QML_ELEMENT

public:
	RayInstancing(QQuick3DObject *parent = nullptr);

	void setRays(const std::vector<DTO_RecievedRay>& rays);
	void clearBuffer();
	void prepareBuffer(int batchSize);
	RayInstancing::InstanceTableEntry makeEntry(const QVector3D& start, const QVector3D& end);

protected:
	QByteArray getInstanceBuffer(int* instanceCount) override;

private:
	QByteArray m_buffer;
	int m_count = 0;
};

