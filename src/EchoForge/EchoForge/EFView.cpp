#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <qicon.h>
#include <QQuickStyle>

#include "EFView.h"
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

	app.setOrganizationName("Projet de synthèse");

	app.setOrganizationDomain("github.com/Cemecla/Acoustic-Tracing");

	app.setApplicationName("Echo Forge");
}

void EFView::setupCommunicator() {

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

}

void EFView::updateListener(const float azimuth, const float elevation) const {
	if (m_audioManager)
	{
		yutasound::SoundRemaining fakeRemaining; 
		m_audioManager->updateSoundSource(0, azimuth, elevation, fakeRemaining);
	}
}

void EFView::play(std::string& url) const {
	m_audioManager->play();
}

void EFView::stop(std::string& url) const {
	m_audioManager->stop();
}

void EFView::buildMap(const ef::Map& map) {
	m_uiCommunicator->buildMap(map);
}