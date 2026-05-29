#pragma once

#include <QtQuick3D/QQuick3DInstancing>
#include <QtQuick3D/QQuick3DObject>
#include "Map.h"

// Documentations
// https://doc.qt.io/qt-6/qtquick3d-custominstancing-example.html
// https://www.cs.cornell.edu/courses/cs3410/2024fa/notes/bitpack.html

class ChunkInstancing : public QQuick3DInstancing
{
	Q_OBJECT
	QML_ELEMENT

public:
	ChunkInstancing(QQuick3DObject *parent = nullptr);

	void setChunk(const ef::Map& map);

	void clearBuffer();

protected:
	QByteArray getInstanceBuffer(int* instanceCount) override;

private:
	QByteArray m_buffer;
	int m_count = 0;
	int id;
};

