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

QVector3D UICommunicator::getWorldBound() const {
	const ef::Map& map = m_parent->getMap();
	float x = (float)map.getWidth();
	float y = (float)map.getHeight();
	float z = (float)map.getDepth();
	return QVector3D{x, y, z};
}

void UICommunicator::startSimulation() {
	m_parent->startSimulation();
}

void UICommunicator::stopSimulation() {
	m_parent->stopSimulation();
	m_parent->unloadRegistry();
	m_rayInstances->clearBuffer();
	m_voxelInstances->clearBuffer();
}

void UICommunicator::saveSimulation(const QString& url) {
	
}

void UICommunicator::loadSimulation(const QString& url) {

	auto _ = QtConcurrent::run([this, url]() {
		try {
			std::string urlconverted = url.toStdString();
			m_parent->loadSimulation(urlconverted);

			QMetaObject::invokeMethod(
				this,
				[this]() { emit simulationLoaded(); },
				Qt::QueuedConnection
			);
		} catch (const std::exception& e) {
			qDebug() << "[loadSimulation] std::exception:" << e.what();
		} catch (Error err) {
			qDebug() << "[loadSimulation] Error:" << err;
		} catch (...) {
			qDebug() << "[loadSimulation] exception inconnue";
		}
	});
}

void UICommunicator::addBlock(const int x, const int y, const int z) {

}

void UICommunicator::destroyBlock(const int x, const int y, const int z) {

}

void UICommunicator::resetEmitters()
{
	m_parent->resetEmitters();
}

void UICommunicator::resumeAll()
{
    m_parent->resumeAll();
}

void UICommunicator::pauseAll()
{
    m_parent->pauseAll();
}

int UICommunicator::playFromFileId(int soundFileId)
{
	return m_parent->playSound(soundFileId);
}

void UICommunicator::resumeEmitter(int emitterId) { m_parent->resumeEmitter(emitterId); }

void UICommunicator::loopEmitter(int emitterId) { m_parent->loopEmitter(emitterId); }
void UICommunicator::pauseEmitter(int emitterId) { m_parent->pauseEmitter(emitterId); }
void UICommunicator::stopEmitter(int emitterId) { m_parent->stopEmitter(emitterId); }

void UICommunicator::updateTestSlider(int emitterId, const float azimuth, const float elevation) {
	m_parent->updateSoundSource(emitterId, azimuth, elevation);
}

void UICommunicator::addSound(const QString& path)
{
	m_parent->addSound(path.toStdString());
}

void UICommunicator::addAmbient(const QString& path)
{
	m_parent->addAmbient(path.toStdString());
}

QVariantList UICommunicator::getSounds()
{
	return m_parent->getSounds();
}

void UICommunicator::loadRegistry(const QString& path)
{
	m_parent->loadRegistry(path.toStdString());
}

void UICommunicator::saveRegistry(const QString& path)
{
	m_parent->saveRegistry(path.toStdString());
}

void UICommunicator::removeSound(int id)
{
	m_parent->removeSound(id);
}

void UICommunicator::unloadRegistry()
{
	m_parent->unloadRegistry();
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

void UICommunicator::savePinnaSetting(QVariantMap& settings) {
	std::string name = settings.value("name", "NoName").toString().toStdString();
	m_parent->savePinnaSetting();
}


QVariantList UICommunicator::loadPinnaSetting() {

	static const std::vector<int> BANDS = { 4000, 6000, 8000, 10000, 12000, 14000, 16000 };

	QVariantList out;

	json settings = m_parent->loadPinnaSetting();

	if (!settings.is_array()) return out;


	for (const auto& entry : settings) {

		if (!entry.is_object()) continue;

		QVariantMap map;


		if (entry.contains("id")) 
			map["id"] = entry["id"].get<int>();

		if (entry.contains("name"))
			map["name"] = QString::fromStdString(entry["name"].get<std::string>());

		if (entry.contains("category"))
			map["category"] = QString::fromStdString(entry["category"].get<std::string>());

		if (entry.contains("cipic_subject"))
			map["cipic_subject"] = entry["cipic_subject"].get<int>();

		if (entry.contains("ear_size_cm"))   
			map["ear_size_cm"] = entry["ear_size_cm"].get<double>();

		if (entry.contains("description"))   
			map["description"] = QString::fromStdString(entry["description"].get<std::string>());



		QVariantList orderedGains;

		if (entry.contains("bands") && entry["bands"].is_object()) {

			for (int hz : BANDS) {
				std::string key = std::to_string(hz);
				if (entry["bands"].contains(key))
					orderedGains.append(entry["bands"][key].get<double>());
				else
					orderedGains.append(0.0);
			}
		}

		map["ordered_gains"] = orderedGains;

		out.append(map);

	}

	return out;

}

void UICommunicator::setSelectedPinnaId(int id) {
	m_parent->setSelectedPinnaId(id);
}

QVariantList UICommunicator::emitters() {
	return m_emittersPositions;
}

void UICommunicator::setEmitters(QVariantList emitters) {
	m_emittersPositions = emitters;
	emittersChanged();
}



