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
struct Inputs;

class EFModel {
	public:
		EFModel(EFController* parent);

		void loadWorld(std::string& filename);

		json loadJson(std::string& filename);

		const std::vector<std::string> getFilesFrom(std::string directoryPath = "./", std::string extensionType = nullptr);

		bool saveJson(const json& data, std::string& filename);

		void update(float delta);

		void startScenario();

		const std::optional<ef::Avatar>& getAvatar() const;

		const ef::Avatar& getAvatar(int temporary) const;

		const ef::Map& getMap() const;

		const std::vector<DTO_RecievedRay>& getDTORays() const;

		[[nodiscard]] const std::vector<ef::Emitter>& getEmitters() const;
		std::vector<ef::Emitter>& getEmittersRef();

		std::vector<ef::Emitter> getActiveSounds();

		void moveAvatar(Inputs& inputs, float delta);

	private:
		const std::string MATERIAL_FILENAME = "";

		EFController* m_parent;
		tracing::RayManager m_rayManager;
		std::optional<ef::EFWorld> m_world;
		// m_scenario déclaré APRÈS m_world : détruit en premier (ordre inverse),
		// joint le thread avant que m_world soit libéré.
		ef::Scenario m_scenario;

		void processBatch();
		void buildScenario();
		int m_debugFrameCount = 0;
};
