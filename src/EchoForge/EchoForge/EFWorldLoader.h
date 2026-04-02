#pragma once
#include "EFWorld.h"

#include "json.hpp"

using json = nlohmann::json;

namespace ef {

	class EFWorldLoader
	{
		public:
			static EFWorld build(const json& mapJson);
			static EFWorld build(const json& mapJson, const json& materialJson);
			static json build(const EFWorld& efWorld);

		private:
			EFWorldLoader() = delete;

			static bool isMaterialsValid(const json& json);
			static bool isMapValid(const json& json);
			static Avatar buildAvatar();
			static Map buildMap();

	};
}