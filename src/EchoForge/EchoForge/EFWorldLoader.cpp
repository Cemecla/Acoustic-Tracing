#include "EFWorldLoader.h"
#include "Utils.h"
#include <vector>
#include <typeinfo>

#include <QDebug>

using namespace ef;
using namespace std;

const Material EFWorldLoader::OUT_MATERIAL = Material("OutOfBound", false, false, Vector3F(1, 1, 1), 1.0, Vector3F(1, 1, 1));

const Material EFWorldLoader::EMPTY_MATERIAL = Material("Empty", true, false, Vector3F(0, 0, 0), 0.0, Vector3F(0, 0, 0));

EFWorld EFWorldLoader::build(const json& mapJson) {
	if (isMapValid(mapJson)) {
		std::string title = mapJson["title"];
		std::string desc = mapJson["desc"];

		json mapData = mapJson["data"];

		std::vector<Material> materials = buildMaterials({});

		Map map = buildMap(mapData, materials);

		Avatar avatar = buildAvatar(mapData);

		return EFWorld(avatar, map);
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

		return EFWorld(avatar, map);
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
	return Avatar(Vector3F(userSpawnPos[0], userSpawnPos[1], userSpawnPos[2]), Vector3F());
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
		std::string name = materialData["name"];
		std::vector<uint8_t> colorBrut = materialData["color"];
		bool isPassable = materialData["isPassable"];
		bool isInstantiable = materialData["isInstantiable"];
		float scatteringCoefficient = materialData["scatteringCoefficient"];
		std::vector<float> normalBrut = materialData["normal"];
		std::vector<float> energyLossBrut = materialData["energyLoss"];

		Vector3F energyLoss = Vector3F(energyLossBrut[0], energyLossBrut[1], energyLossBrut[2]);
		Vector3F normal = Vector3F(normalBrut[0], normalBrut[1], normalBrut[2]);
		ColorRGBA colorRGB = ColorRGBA(colorBrut[0], colorBrut[1], colorBrut[2], colorBrut[3]);

		Material nm = Material(name, isPassable, isInstantiable, energyLoss, scatteringCoefficient, normal, colorRGB);

		materials.push_back(nm);
	}

	return materials;
}