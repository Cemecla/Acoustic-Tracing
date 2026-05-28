#include "EFWorldLoader.h"
#include "Utils.h"
#include <vector>
#include <typeinfo>

#include <QDebug>

using namespace ef;
using namespace std;

const Material EFWorldLoader::OUT_MATERIAL = Material("OutOfBound", false, false, Vector3F(1, 1, 1), 1.0, Vector3F(1, 1, 1), 0,0,0);

const Material EFWorldLoader::EMPTY_MATERIAL = Material("Empty", true, false, Vector3F(0.002, 0.001, 0.0005), 0.0, Vector3F(0, 0, 0), 0, 0, 0);

EFWorld EFWorldLoader::build(const json& mapJson) {
	if (isMapValid(mapJson)) {
		std::string title = mapJson["title"];
		std::string desc = mapJson["desc"];

		json mapData = mapJson["data"];

		std::vector<Material> materials = buildMaterials({});

		Map map = buildMap(mapData, materials);

		Avatar avatar = buildAvatar(mapData);

		EFWorld world(avatar, map);
		world.emitters = buildEmitters(mapJson);
		return world;
	}
	else
	{
		throw Error::ERROR_INVALID_DATA;
	}
	return EFWorld();
}

EFWorld EFWorldLoader::build(const json& mapJson, const json& materialJson) {
	if (isMapValid(mapJson) && isMaterialsValid(materialJson)) {
		std::string title = mapJson["title"];
		std::string desc = mapJson["desc"];

		json mapData = mapJson["data"];

		std::vector<Material> materials = buildMaterials(materialJson);

		Map map = buildMap(mapData, materials);

		Avatar avatar = buildAvatar(mapData);

		EFWorld world(avatar, map);
		world.emitters = buildEmitters(mapJson);
		return world;
	}
	else
	{
		throw Error::ERROR_INVALID_DATA;
	}
	return EFWorld();
}

bool EFWorldLoader::isMapValid(const json& jsonData) {

	if (isValid<string>(jsonData, "title")) {
		qDebug() << true;
	}

	if (isValid<string>(jsonData, "desc")) {
		qDebug() << true;
	}

	if (isValid<nlohmann::json>(jsonData, "data")) {
		json dataVoxels = jsonData["data"];
		if (isValid<unsigned>(dataVoxels, "voxelSize")) {
			qDebug() << true;
		}

		if (isValid<unsigned>(dataVoxels, "chunkSize")) {
			qDebug() << true;
		}

		if (isValid<unsigned>(dataVoxels, "mapWidth")) {
			qDebug() << true;
		}

		if (isValid<unsigned>(dataVoxels, "mapHeight")) {
			qDebug() << true;
		}

		if (isValid<unsigned>(dataVoxels, "mapDepth")) {
			qDebug() << true;
		}

		if (isValid<vector<float>>(dataVoxels, "userSpawnPos")) {
			qDebug() << true;
		}

		if (isValid<vector<int>>(dataVoxels, "voxel")) {
			qDebug() << true;
		}
	}
	return true;
}

template<typename T>
bool EFWorldLoader::isValid(const json& jsonData, string key) {
	if (jsonData.contains(key))
		if (typeid(jsonData[key]) == typeid(T))
			return true;
	qDebug() << false;
	return false;
}

bool EFWorldLoader::isMaterialsValid(const json& jsonData) {
	return true;
}

Avatar EFWorldLoader::buildAvatar(const json& mapData) {
	std::vector<float> userSpawnPos = mapData["userSpawnPos"];
	const float voxelUnitSize = mapData.value("voxelUnitSize", 1.0f);
	return Avatar(Vector3F(userSpawnPos[0], userSpawnPos[1], userSpawnPos[2] ), Vector3F());
}

std::vector<int> EFWorldLoader::searchValidVoxels(std::vector<int> voxels, std::vector<Material> materials) {
	std::vector<int> validMaterialsId = {};
	std::vector<int> validVoxelsId = {};

	for (int i = 0; i < materials.size(); i++) {
		if (materials[i].isInstantiable) {
			validMaterialsId.push_back(i);
		}
	}

	for (int i = 0; i < voxels.size(); i++) {
		auto iter = std::find(validMaterialsId.begin(), validMaterialsId.end(), voxels[i]);

		if (iter != validMaterialsId.end())
		{
			validVoxelsId.push_back(i);
		}
	}

	return validMaterialsId;
}

Map EFWorldLoader::buildMap(const json& mapData, std::vector<Material> materials) {
	int voxelUnitSize = mapData["voxelUnitSize"];

	int unitSize = mapData["unitValue"];

	unsigned mapWidth = mapData["mapWidth"];

	unsigned mapHeight = mapData["mapHeight"];

	unsigned mapDepth = mapData["mapDepth"];

	std::vector<int> voxels = mapData["voxel"];

	return Map(mapWidth, mapHeight, mapDepth, voxelUnitSize, unitSize, voxels, materials);
}

std::vector<Material> EFWorldLoader::buildMaterials(const json& materialJson) {
	std::vector<Material> materials = {};
	materials.push_back(OUT_MATERIAL);
	materials.push_back(EMPTY_MATERIAL);

	for(json materialData : materialJson)
	{
		std::string name = materialData.value("name", "NoName");
		std::vector<uint8_t> colorBrut = materialData["color"];
		bool isPassable = materialData.value("isPassable", false);
		bool isInstantiable = materialData.value("isInstantiable", true);
		float scatteringCoefficient = materialData["scatteringCoefficient"];
		std::vector<float> normalBrut = materialData["normal"];
		std::vector<float> energyLossBrut = materialData["energyLoss"];

		uint8_t topTile = 0;
		uint8_t sideTile = 0;
		uint8_t bottomTile = 0;

		if (materialData.contains("tiles")) {
			const auto& tiles = materialData["tiles"];
			topTile = tiles.value("top", (uint8_t)0);
			sideTile = tiles.value("sides", (uint8_t)0);
			bottomTile = tiles.value("bottom", (uint8_t)0);
		}

		float metalness = materialData["metalness"];
		float specular = materialData["specular"];
		float roughness = materialData["roughness"];
		// transmissionCoefficient est optionnel — défaut 0 (opaque au son) si absent du JSON
		float transmissionCoefficient = materialData.value("transmissionCoefficient", 0.0f);
		// transmissionLoss : perte unique appliquée à l'entrée en mode transmission, optionnelle
		auto transmissionLossBrut = materialData.value("transmissionLoss", std::vector<float>{0.f, 0.f, 0.f});

		Vector3F energyLoss = Vector3F(energyLossBrut[0], energyLossBrut[1], energyLossBrut[2]);
		Vector3F normal = Vector3F(normalBrut[0], normalBrut[1], normalBrut[2]);
		ColorRGBA colorRGB = ColorRGBA(colorBrut[0], colorBrut[1], colorBrut[2], colorBrut[3]);
		Vector3F transmissionLoss = Vector3F(transmissionLossBrut[0], transmissionLossBrut[1], transmissionLossBrut[2]);

		Material nm = Material(name, isPassable, isInstantiable, energyLoss, scatteringCoefficient, normal, topTile, sideTile, bottomTile, colorRGB, roughness, metalness, specular, transmissionCoefficient, transmissionLoss);
		materials.push_back(nm);
	}

	return materials;
}

std::vector<Emitter> EFWorldLoader::buildEmitters(const json& mapJson) {
	std::vector<Emitter> emitters;

	if (!mapJson.contains("scenarioItems")) return emitters;

	const auto& items = mapJson["scenarioItems"];
	int id = 100;

	for (const auto& item : items) {
		std::string filename = "./res/assets/sounds/"+item.value("filename", "");
		float volume         = item.value("volume", 1.0f);
		bool loop            = item.value("loop", false);
		int soundId = item.value("id", id);

		Vector3F position;
		if (item.contains("position")) {
			std::vector<float> pos = item["position"];
			// Positions dans le JSON sont en coordonnées monde (mêmes unités que l'avatar).
			// NE PAS diviser par voxelUnitSize — buildAvatar() utilise aussi les coords brutes.
			position = Vector3F(pos[0], pos[1], pos[2]);
		}

		EventType eventType  = EventType::COLLISION;
		float triggerTime    = 0.0f;
		Vector3F triggerDelta;

		if (item.contains("event")) {
			const auto& event = item["event"];
			std::string typeStr = event.value("type", "collision");

			if (typeStr == "timer") {
				eventType   = EventType::TIME;
				triggerTime = event.value("time", 0.0f) / 1000.0f;
			}
			else if (typeStr == "collision") {
				if (event.contains("delta")) {
					std::vector<float> d = event["delta"];
					triggerDelta = Vector3F(d[0], d[1], d[2]);
				}
			}
		}
		// appeller Librarian pour avoir les id des sons et non pas id


		emitters.push_back(Emitter{ id, soundId, filename, position, volume, loop, false, eventType, triggerTime, triggerDelta });
		id++;
	}

	//qDebug() << "[EFWorldLoader] scenarioItems chargés :" << emitters.size();
	return emitters;
}
