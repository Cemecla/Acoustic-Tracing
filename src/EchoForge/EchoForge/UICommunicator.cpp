#include <cctype>
#include <iostream>
#include <QDebug>
#include <QtConcurrent>
#include <QMetaObject>
#include <Eigen/Dense>

#include "UICommunicator.h"
#include "EFView.h"
#include "Material.h"
#include "DTO_RecievedRay.h"


UICommunicator::UICommunicator(EFView* view) : m_parent(view), m_pos(QVector3D()), m_angles(QVector3D()), m_voxelInstances(new ChunkInstancing()), m_rayInstances(new RayInstancing()) {}

UICommunicator::UICommunicator(EFView* view, ef::Vector3F pos, ef::Vector3F angles) : m_parent(view), m_pos(QVector3D(pos.x, pos.y, pos.z)), m_angles(QVector3D(angles.x, angles.y, angles.z)), m_voxelInstances(new ChunkInstancing()), m_rayInstances(new RayInstancing()){}

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

ChunkInstancing* UICommunicator::voxelInstances() {
	return m_voxelInstances;
}

RayInstancing* UICommunicator::rayInstances() {
	return m_rayInstances;
}

void UICommunicator::startSimulation() {
	m_parent->startSimulation();
}

void UICommunicator::stopSimulation() {

}

void UICommunicator::saveSimulation(const QString& url) {
	
}

void UICommunicator::loadSimulation(const QString& url) {

	auto _ = QtConcurrent::run([this, url]() {

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

void UICommunicator::userInput(int key, bool pressed) {
	switch (key) {
		case Qt::Key_W: m_inputs.forward = pressed; break;
		case Qt::Key_S: m_inputs.backward = pressed; break;
		case Qt::Key_A: m_inputs.left = pressed; break;
		case Qt::Key_D: m_inputs.right = pressed; break;
		case Qt::Key_Q: m_inputs.rotLeft = pressed; break;
		case Qt::Key_E: m_inputs.rotRight = pressed; break;
		default: break;
	}
}

const Inputs& UICommunicator::getInputs() const {
	return m_inputs;
}

void UICommunicator::updateRayInstances(const std::vector<DTO_RecievedRay>& rays) {

	m_rayInstances->setRays(rays);
}

void UICommunicator::buildMap(const ef::Map& map) {	
	m_voxelInstances->setChunk(map);
}

void UICommunicator::setupAvatar(const ef::Avatar& avatar) {
	ef::Vector3 pos = avatar.getPosition();
	QVector3D qpos = QVector3D(pos.x, pos.y, pos.z);
	setAvatarPos(qpos);

	ef::Vector3 angle = avatar.getHeadAngles();
	QVector3D qangle = QVector3D(angle.x, angle.y, angle.z);
	setHeadAngles(qangle);
}


void UICommunicator::callSimulationLoaded() {
	emit simulationLoaded();
}