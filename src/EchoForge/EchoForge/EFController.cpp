#include "EFController.h"
#include "EFModel.h"
#include "EFView.h"

// Constructor
EFController::EFController() {
	model_ = new EFModel(this); 
	view_ = new EFView(this);
}

// Deconstructor
EFController::~EFController() {
	delete model_;
	delete view_;
}