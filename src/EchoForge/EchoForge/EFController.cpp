#include "EFController.h"
#include "EFModel.h"
#include "EFView.h"

// Constructor
EFController::EFController(): m_model(std::make_unique<EFModel>(this)), m_view(std::make_unique<EFView>(this)) {

}

bool EFController::run() {
	return true;
}

void EFController::loadSimulation(std::string& url) {
	m_model->loadWorld(url);
}