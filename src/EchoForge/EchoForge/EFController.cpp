#include "EFController.h"
#include "EFModel.h"
#include "EFView.h"

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
	m_view->buildMap(map);
	m_simulationLoop.start();
}

void EFController::startSimulation() {
	m_simulationLoop.start();
}