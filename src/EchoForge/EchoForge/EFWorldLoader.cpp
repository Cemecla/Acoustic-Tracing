#include "EFWorldLoader.h"
#include "Utils.h"

#include <QDebug>

using namespace ef;

EFWorld EFWorldLoader::build(const json& mapJson) {
	if (isMapValid(mapJson)) {
		std::string title = mapJson["title"];
		std::string desc = mapJson["desc"];
		json mapData = mapJson["data"];

		int voxelSize = mapData["voxelSize"];

		qDebug() << title;
		qDebug() << desc;
		qDebug() << voxelSize;
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
//
bool EFWorldLoader::isMapValid(const json& json) {
	return true;
}
//
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