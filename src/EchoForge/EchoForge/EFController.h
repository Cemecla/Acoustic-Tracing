#pragma once
#include <memory>
#include <string>
#include "SimulationLoop.h"

class EFModel;
class EFView;
struct Inputs;

#ifndef EF_CONTROLLER_H
	#define EF_CONTROLLER_H
	class EFController {
		public:
			/**
			*	Default constructor
			*/
			EFController();
			std::unique_ptr<EFModel> m_model;
			std::unique_ptr<EFView> m_view;

			void run(float delta);
			void loadSimulation(std::string& url);
			void startSimulation();
			void updateViewRays();
			void activateEmitter(int index);
			void updateEmitterListener(int audioEmitterId, float azimuth, float elevation);
			const Inputs& getLastInputs() const;
	
		private:
			SimulationLoop m_simulationLoop;
	};
	
#endif