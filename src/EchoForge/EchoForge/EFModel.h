#pragma once

#include "Global.h"

#include <optional>
#include <vector>

#include "Map.h"
#include "Avatar.h"
#include "RayManager.h"
#include "EFWorld.h"
#include "Scenario.h"
#include "AcousticInterpreter.h"
#include "Vector.h"
#include "TreeMap.h"

class EFController;
struct Inputs;


// IA Usage
// Utilisation: Writing documentation
// Model: Claude Sonnet 4.6
// Prompt: Analyze the provided C++ header file and add professional documentation 
//         comments for all classes, structs, enums, constructors, destructors, 
//         properties, and public methods.
// Developper: Joël Mailly-Niquay

#ifndef EF_MODEL_H
	#define EF_MODEL_H

	class EFModel {
		public:
			/**
			*	Main constructor
			*	desc: The EFModel is where the logic reside and take the Controller as the parent
			*   param parent: EFController
			*/
			EFModel(EFController* parent);

			void loadWorld(std::string& filename);

			/**
			*	desc:
			*   param name: string
			*	return: json
			*/
			json loadJson(std::string& filename);

			/**
			*	desc:
			*	param data: json
			*   param name: string
			*/
			bool saveJson(const json& data, std::string& filename);

			/**
			*	desc:
			*	param data: json
			*   param name: string
			*/
			void update(float delta);

			void startScenario();

			void stopScenario();

			const std::optional<ef::Avatar>& getAvatar() const;

			const ef::Map& getMap() const;

			const std::vector<DTO_RecievedRay>& getDTORays() const;

			[[nodiscard]] const std::vector<ef::Emitter>& getEmitters() const;

			std::vector<ef::Emitter> getActiveSounds();

			const std::vector<yutasound::AmbientEntry>& getAmbientEntries() const;

			void moveAvatar(Inputs& inputs, float delta);
			
			void endWorld();
			
			const float getRefreshTime() const;
			void setRefreshTime(float timeInSeconde);

			const int getMaxRayCount() const;
			void setMaxRayCount(int rayCount);

			const int getRayPerFrame() const;
			void setRayPerFrame(int rayPerFrame);
		
		private:
			const std::string MATERIAL_FILENAME = "";

			EFController* m_parent;
			tracing::RayManager m_rayManager;
			AcousticInterpreter m_acousticInterpreter;
			std::optional<ef::EFWorld> m_world;
			// m_scenario déclaré APRÈS m_world : détruit en premier (ordre inverse),
			// joint le thread avant que m_world soit libéré.
			ef::Scenario m_scenario;

			void processBatch();
			void buildScenario();
			int m_debugFrameCount = 0;
			float m_updateRayTimer = 0.0;
			float m_refreshTime = 0.5;
			ef::TreeMap<int, int> m_emitterIdMap; // world id -> SAE emitter id
	};

#endif
