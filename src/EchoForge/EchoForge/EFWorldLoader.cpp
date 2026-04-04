#include "EFWorldLoader.h"
#include "Utils.h"
#include <vector>

#include <QDebug>

using namespace ef;

EFWorld EFWorldLoader::build(const json& mapJson) {
	if (isMapValid(mapJson)) {
		std::string title = mapJson["title"];
		std::string desc = mapJson["desc"];

		json mapData = mapJson["data"];

		float voxelSize = mapData["voxelSize"];

		unsigned mapWidth = mapData["mapWidth"];

		unsigned mapHeight = mapData["mapHeight"];

		unsigned mapDepth = mapData["mapDepth"];

		std::vector<float> userSpawnPos = mapData["userSpawnPos"];

		std::vector<int> voxels = mapData["voxel"];

		Map map = Map(mapWidth, mapHeight, mapDepth, voxelSize, voxels);

		Avatar avatar = Avatar(Vector3F(userSpawnPos[0], userSpawnPos[1], userSpawnPos[2]), Vector3F());

		return EFWorld(avatar, map);
	}
	else
	{
		throw Error::ERROR_INVALID_DATA;
	}
	return EFWorld();
}

//EFWorld EFWorldLoader::build(const json& mapJson, const json& materialJson) {
//	isMapValid(mapJson);
//	isMaterialsValid(materialJson);
//}

bool EFWorldLoader::isMapValid(const json& json) {
	return true;
}

//bool EFWorldLoader::isMaterialsValid(const json& json) {
//
//}
//
//Avatar EFWorldLoader::buildAvatar() {
//
//}
//
//Map EFWorldLoader::buildMap() {
//
//}