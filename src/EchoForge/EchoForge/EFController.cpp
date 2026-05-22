#include "EFController.h"
#include "EFModel.h"
#include "EFView.h"
#include "Utils.h"
#include <qdebug.h>

// Constructor
EFController::EFController(): m_model(std::make_unique<EFModel>(this)), m_view(std::make_unique<EFView>(this)), m_simulationLoop(SimulationLoop()) {
	m_simulationLoop.setCallback([this](float delta) {
		run(delta);
	});
}

void EFController::run(float delta) {
	m_model->update(delta);
	m_view->update(delta);
}

void EFController::loadSimulation(std::string& url) {
	m_model->loadWorld(url);
	ef::Map map = m_model->getMap();
	ef::Avatar avatar = m_model->getAvatar(true);
	m_view->buildMap(map);
	m_view->setupAvatar(avatar);
	m_view->setEmittersVisual(m_model->getEmitters());
}

const Inputs& EFController::getLastInputs() const {
	return m_view->getLastInput();
}

void EFController::updateViewRays() {
	m_view->updateRayInstance();
}

void EFController::startSimulation() {
	m_model->startScenario();
	m_simulationLoop.start();
}

void EFController::stopSimulation() {
	m_simulationLoop.stop();
}

void EFController::pauseSimulation() {

}