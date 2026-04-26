#pragma once

#include "json.hpp"
#include <string>
#include <optional>
#include <vector>

#include "Map.h"
#include "Avatar.h"
#include "RayManager.h"
#include "EFWorld.h"
#include "Scenario.h"

using json = nlohmann::json;

class EFController;

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

			const std::vector<std::string> getFilesFrom(std::string directoryPath = "./", std::string extensionType = nullptr);
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

			const std::optional<ef::Avatar>& getAvatar() const;

			const ef::Avatar& getAvatar(int temporary) const;

			const ef::Map& getMap() const;

			[[nodiscard]] const std::vector<ef::Emitter>& getEmitters() const;

			std::vector<ef::Emitter> getActiveSounds();


		private:
			const std::string MATERIAL_FILENAME = "";

			EFController* m_parent;
			tracing::RayManager m_rayManager;
			std::optional<ef::EFWorld> m_world;
			// m_scenario déclaré APRÈS m_world : détruit en premier (ordre inverse),
			// joint le thread avant que m_world soit libéré.
			ef::Scenario m_scenario;

			// TODO: remplacer par le vrai processing audio
			void processBatch();
			void buildScenario();
			int m_debugFrameCount = 0;
	};

#endif
