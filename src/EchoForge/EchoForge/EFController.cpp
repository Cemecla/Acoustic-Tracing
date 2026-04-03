#include "EFController.h"
#include "EFModel.h"
#include "EFView.h"
#include <QDebug>

// Constructor
EFController::EFController(): m_model(std::make_unique<EFModel>(this)), m_view(std::make_unique<EFView>(this)), m_simulationLoop(SimulationLoop()) {
	m_simulationLoop.setCallback([this](float delta) {
		run(delta);
	});

	// m_simulationLoop.start();
}

void EFController::run(float delta) {
	qDebug() << delta;
}

void EFController::loadSimulation(std::string& url) {
	m_model->loadWorld(url);
}