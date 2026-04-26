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
			static const Material EMPTY_MATERIAL;
			static const Material OUT_MATERIAL;

			EFWorldLoader() = delete;

			static bool isMaterialsValid(const json& jsonData);
			static bool isMapValid(const json& jsonData);

			template<typename T>
			static bool isValid(const json& jsonData, std::string key);

			static Avatar buildAvatar(const json& mapData);
			static Map buildMap(const json& mapData, std::vector<Material> materials);
			static std::vector<Material> buildMaterials(const json& materialJson);
			static std::vector<Emitter> buildEmitters(const json& mapJson);

			static std::vector<int> searchValidVoxels(std::vector<int> voxels, std::vector<Material> materials);
	};
}
