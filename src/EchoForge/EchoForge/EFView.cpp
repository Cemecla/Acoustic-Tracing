#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <qicon.h>
#include <QQuickStyle>
#include <qvector.h>
#include <QVariantList> 
#include <QVariantMap>

#include "json.hpp"

#include "EFView.h"
#include "EFModel.h"
#include "EFController.h"
#include "UICommunicator.h"

EFView::EFView(EFController* parent) : m_parent(parent), m_qtEngine(new QQmlApplicationEngine), m_uiCommunicator(new UICommunicator(this)) {
	QQmlContext* context = m_qtEngine->rootContext();

	context->setContextProperty("communicator", m_uiCommunicator);

	QQuickStyle::setStyle("Basic");

	m_qtEngine->load(QUrl(QStringLiteral("qrc:/qt/qml/echoforge/main.qml")));
}

void EFView::setupAudioManager(yutasound::AudioManager* audioManager) {
	m_audioManager = audioManager;
}

void EFView::setupQGuiApplication(QGuiApplication& app) const {
	app.setWindowIcon(QIcon(":/images/logo"));

	app.setOrganizationName("Rotten Peach");

	app.setOrganizationDomain("rotten-peach.com");

	app.setApplicationName("Echo Forge");
}

void EFView::setupAvatar(const ef::Avatar& avatar) {
	m_uiCommunicator->setupAvatar(avatar);
}

void EFView::startSimulation() {
	m_parent->startSimulation();
}

void EFView::saveSimulation() {

}

void EFView::stopSimulation() {
	m_parent->stopSimulation();
	unloadRegistry();
	resetEmitters();
}

void EFView::pauseSimulation() {

}

void EFView::saveSimulation(std::string& url) {

}

void EFView::loadSimulation(std::string& url) {
	m_parent->loadSimulation(url);
}

void EFView::simulationLoaded() const {
	m_uiCommunicator->callSimulationLoaded();
}

void EFView::update(float delta) {
	const ef::Avatar& avatar = *m_parent->m_model->getAvatar();

	ef::Vector3F position = avatar.getPosition();
	QVector3D qpos = { position.x, position.y, position.z };
	m_uiCommunicator->setAvatarPos(qpos);

	ef::Vector3F angles = avatar.getHeadAngles();

	constexpr float RAD_TO_DEG = 57.2957795f;

	QVector3D qangles = {
		angles.x * RAD_TO_DEG,
		angles.y * RAD_TO_DEG,
		angles.z * RAD_TO_DEG
	};

	m_uiCommunicator->setHeadAngles(qangles);
}

void EFView::updateRayInstance() {
	const auto& rays = m_parent->m_model->getDTORays();
	m_uiCommunicator->updateRayInstances(rays);
}


// AUDIOMANAGER
void EFView::updateSoundSource(int emitterId, float azimuth, float elevation) const {
	if (m_audioManager && emitterId != -1)
	{
		yutasound::SoundRemaining fakeRemaining;
		m_audioManager->updateSoundSource(emitterId, azimuth, elevation, fakeRemaining);
	}
}

void EFView::addSound(const std::string& path) const
{
	if (m_audioManager)
		m_audioManager->registerSound(juce::String(path));
}

int EFView::playSound(int soundFileId) const
{
	if (m_audioManager)
	{
		yutasound::SoundRemaining defaultRemaining;
		int emitterId = m_audioManager->playSound(soundFileId, 0.0f, 0.0f, defaultRemaining);
		m_audioManager->play();
		return emitterId;
	}
	return -1;
}

int EFView::registerSound(const std::string& path) const {
	if (m_audioManager)
		return m_audioManager->registerSound(juce::String(path));
	return -1;
}

int EFView::playSound(int soundFileId, float azimuth, float elevation, yutasound::SoundRemaining remaining) const {
	if (m_audioManager)
	{
		int emitterId = m_audioManager->playSound(soundFileId, azimuth, elevation, remaining);
		m_audioManager->play();
		return emitterId;
	}
	return -1;
}

void EFView::updateSoundSource(int emitterId, float azimuth, float elevation, yutasound::SoundRemaining remaining) const {
	if (m_audioManager)
		m_audioManager->updateSoundSource(emitterId, azimuth, elevation, remaining);
}

void EFView::stopAudio() const {
	if (m_audioManager)
		m_audioManager->stop();
}

QVariantList EFView::getSounds() const
{
	QVariantList list;
	if (m_audioManager)
	{
		for (const auto& entry : m_audioManager->getEntries())
		{
			QVariantMap map;
			map["id"] = entry.id;
			map["path"] = QString::fromStdString(entry.filePath.toStdString());
			list.append(map);
		}
	}
	return list;
}

void EFView::resetEmitters() const {
	if (m_audioManager)
		m_audioManager->reset();
}

void EFView::resumeAll() const {
	if (m_audioManager)
		m_audioManager->resumeAll();
}

void EFView::pauseAll() const {
	if (m_audioManager)
		m_audioManager->pauseAll();
}

void EFView::resumeEmitter(int emitterId) const {
	if (m_audioManager) m_audioManager->resumeSound(emitterId);
}

void EFView::pauseEmitter(int emitterId) const {
	if (m_audioManager) m_audioManager->pauseSound(emitterId);
}

void EFView::stopEmitter(int emitterId) const {
	if (m_audioManager) m_audioManager->stopSound(emitterId);
}

void EFView::loadRegistry(const std::string& path) const {
	if (m_audioManager)
		m_audioManager->loadSoundRegistry(juce::String(path));
}

void EFView::saveRegistry(const std::string& path) const {
	if (m_audioManager)
		m_audioManager->saveSoundRegistry(juce::String(path));
}

void EFView::removeSound(int id) const {
	if (m_audioManager)
		m_audioManager->removeSound(id);
}

void EFView::unloadRegistry() const {
	if (m_audioManager)
	{
		m_audioManager->stop();
		juce::Thread::sleep(20);
		m_audioManager->unloadSoundRegistry();
	}
}

void EFView::buildMap(const ef::Map& map) {
	m_uiCommunicator->buildMap(map);
}

const Inputs& EFView::getLastInput() const {
	return m_uiCommunicator->getInputs();
}

json EFView::loadPinnaSetting() {
	return json{};
}

void EFView::savePinnaSetting() {

}