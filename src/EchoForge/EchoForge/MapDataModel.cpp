#include "MapDataModel.h"
#include <QtPreprocessorSupport>

MapDataModel::MapDataModel(QObject* parent) : QAbstractListModel(parent) {

}

int MapDataModel::rowCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return m_mapData.size();
}

QVariant MapDataModel::data(const QModelIndex& index, int role) const {
	const MapData& mapData = m_mapData[index.row()];

	//switch (role)
	//{

	//}

	return {};
}

QHash<int, QByteArray> MapDataModel::roleNames() const {
	return {};
}