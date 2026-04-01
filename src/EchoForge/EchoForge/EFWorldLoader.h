#pragma once

#include "json.hpp"

using json = nlohmann::json;

namespace ef {

	struct EFWorld;
	class Avatar;
	class Map;

	class EFWorldLoader
	{
		public:
			static EFWorld build(const json& mapJson);
			static EFWorld build(const json& mapJson, const json& materialJson);

		private:
			EFWorldLoader() = delete;

			static bool isMaterialsValid(const json& json);
			static bool isMapValid(const json& json);
			static Avatar buildAvatar();
			static Map buildMap();

	};
}