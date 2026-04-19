#pragma once

#include <QtQuick3D/QQuick3DInstancing>
#include <QtQuick3D/QQuick3DObject>

class RayInstancing : public QQuick3DInstancing
{
	Q_OBJECT
	QML_ELEMENT

public:
	RayInstancing(QQuick3DObject *parent = nullptr);
	~RayInstancing();

	void setRays();

protected:
	QByteArray getInstanceBuffer(int* instanceCount) override;

private:
	QByteArray m_buffer;
	int m_count = 0;
};

