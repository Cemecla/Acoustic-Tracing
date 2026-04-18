#include <cctype>
#include <iostream>
#include <QDebug>
#include <QtConcurrent>
#include <QMetaObject>

#include "UICommunicator.h"
#include "EFView.h"
#include "Material.h"


UICommunicator::UICommunicator(EFView* view) : m_parent(view), m_pos(QVector3D()), m_angles(QVector3D()), m_voxelInstances(new ChunkInstancing()) {}

UICommunicator::UICommunicator(EFView* view, ef::Vector3F pos, ef::Vector3F angles) : m_parent(view), m_pos(QVector3D(pos.x, pos.y, pos.z)), m_angles(QVector3D(angles.x, angles.y, angles.z)), m_voxelInstances(new ChunkInstancing()) {}

void UICommunicator::setAvatarPos(QVector3D& v) {
	m_pos = v;
	emit avatarPosChanged();
}

void UICommunicator::setHeadAngles(QVector3D& v) {
	m_angles = v;
	emit headAnglesChanged();
}

QVector3D UICommunicator::avatarPos() const {
	return m_pos;
}

QVector3D UICommunicator::headAngles() const {
	return m_angles;
}

ChunkInstancing* UICommunicator::voxelInstances()
{
	return m_voxelInstances;
}

void UICommunicator::startSimulation() {
	m_parent->startSimulation();
}

void UICommunicator::stopSimulation() {

}

void UICommunicator::saveSimulation(const QString& url) {
	
}

void UICommunicator::loadSimulation(const QString& url) {

	QtConcurrent::run([this, url]() {

		std::string urlconverted = url.toStdString();
		m_parent->loadSimulation(urlconverted);

		QMetaObject::invokeMethod(
			this,
			[this]() { emit simulationLoaded(); },
			Qt::QueuedConnection
		);
	});
}

void UICommunicator::addBlock(const int x, const int y, const int z) {

}

void UICommunicator::destroyBlock(const int x, const int y, const int z) {

}

void UICommunicator::testing(const QString& value) {
	qDebug() << value;
}

void UICommunicator::playtest(const QString& url) {
	std::string urlconverted = url.toStdString();
	m_parent->play(urlconverted);
}

void UICommunicator::stoptest(const QString& url) {
	std::string urlconverted = url.toStdString();
	m_parent->stop(urlconverted);
}

void UICommunicator::updateTestSlider(const float azimuth, const float elevation) {

	m_parent->updateListener(azimuth, elevation);
}

void UICommunicator::userInput(char input) {
	input = std::tolower(input);

	switch (input)
	{
		case 'a':
			break;
		case 'd':
			break;
		default:
			break;
	}
}

void UICommunicator::buildMap(ef::Map& map) {	
	m_voxelInstances->setChunk(map);
}

void UICommunicator::callSimulationLoaded() {
	emit simulationLoaded();
}