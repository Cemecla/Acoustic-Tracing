#include <QQmlApplicationEngine>

#include "EFView.h"
#include "EFController.h"
#include "UICommunicator.h"

EFView::EFView(EFController* parent) : m_parent(parent), m_qtEngine(new QQmlApplicationEngine) {
	m_qtEngine->load(QUrl(QStringLiteral("qrc:/qt/qml/echoforge/main.qml")));
}

void EFView::setupCommunicator() {

}

void EFView::startSimulation() {

}

void EFView::saveSimulation() {

}

void EFView::stopSimulation(){}

void EFView::saveSimulation(std::string url) {

}

void EFView::loadSimulation(std::string url){}

void EFView::update() {

}