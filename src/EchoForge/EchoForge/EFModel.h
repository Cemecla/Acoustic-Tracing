#pragma once

#include "json.hpp"
#include <string>
#include <optional>
#include <vector>

#include "Map.h"
#include "Avatar.h"
#include "RayManager.h"
#include "EFWorld.h"

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
	
			const std::optional<ef::Avatar>& getAvatar() const;

			const ef::Map& getMap() const;

			std::vector<ef::Emitter> getActiveSounds();
			
		
		private:
			const std::string MATERIAL_FILENAME = "";

			EFController* m_parent;
			tracing::RayManager m_rayManager;
			std::optional<ef::EFWorld> m_world;
	};

#endif
