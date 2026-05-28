#pragma once
#include "Global.h"

#include <memory>
#include "SimulationLoop.h"

class EFModel;
class EFView;
struct Inputs;

// IA Usage
// Utilisation: Writing documentation
// Model: Claude Sonnet 4.6
// Prompt: Analyze the provided C++ header file and add professional documentation 
//         comments for all classes, structs, enums, constructors, destructors, 
//         properties, and public methods.
// Developper: Joël Mailly-Niquay

#ifndef EF_CONTROLLER_H
	#define EF_CONTROLLER_H
	/**
	*	@class EFController
	*	@brief Central controller coordinating the model and view in the MVC architecture.
	*
	*	Owns the EFModel and EFView instances and mediates communication between them.
	*	Drives the simulation through a SimulationLoop that periodically invokes run()
	*	to update both the model and view with the elapsed time delta.
	*/
	class EFController {
		public:
			/**
			*	@summary Constructs the controller and instantiates its model and view.
			*	@details Initializes the model and view with a back-reference to this controller,
			*	         and registers run() as the simulation loop callback.
			*/
			EFController();

			/** @brief Owned model instance handling simulation state and logic. */
			std::unique_ptr<EFModel> m_model;

			/** @brief Owned view instance handling rendering and user input. */
			std::unique_ptr<EFView> m_view;


			/**
			*	@summary Advances the simulation by one tick.
			*	@param delta Elapsed time in seconds since the previous tick.
			*	@details Called by the SimulationLoop; updates the model first, then the view.
			*/
			void run(float delta);

			/**
			*	@summary Loads a simulation scenario from the given source.
			*	@param url Path or URL identifying the world/scenario to load.
			*	@details Loads the world into the model, then builds the map, sets up the
			*	         avatar, and configures emitter visuals in the view.
			*/
			void loadSimulation(std::string& url);

			/**
			*	@summary Starts the simulation scenario and begins the update loop.
			*/
			void startSimulation();

			/**
			*	@summary Stops the simulation loop.
			*/
			void stopSimulation();

			/**
			*	@summary Pauses the simulation.
			*	@note Currently a stub; no behavior implemented.
			*/
			void pauseSimulation();


			/**
			*	@summary Refreshes the view's ray instance rendering.
			*	@details Delegates to the view to update its ray visualization.
			*/
			void updateViewRays();

			/**
			*	@summary Returns the most recent user inputs captured by the view.
			*	@return Const reference to the last Inputs struct from the view.
			*/
			const Inputs& getLastInputs() const;
	
		private:
			/** @brief Loop driving periodic calls to run() with a time delta. */
			SimulationLoop m_simulationLoop;
	};
	
#endif