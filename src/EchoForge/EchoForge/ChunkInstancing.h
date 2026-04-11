#pragma once

#include <QtQuick3D/QQuick3DInstancing>
#include <QtQuick3D/QQuick3DObject>
#include "Map.h"

class ChunkInstancing  : public QQuick3DInstancing
{
	Q_OBJECT
	QML_ELEMENT

public:
	ChunkInstancing(QQuick3DObject *parent = nullptr);

	void setChunk(const ef::Map& map);

protected:
	QByteArray getInstanceBuffer(int* instanceCount) override;

private:
	QByteArray m_buffer;
	int m_count = 0;
};

