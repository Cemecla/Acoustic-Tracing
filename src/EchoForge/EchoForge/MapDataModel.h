#pragma once
#include <QAbstractListModel>
#include <vector>

struct MapData {
	QString name;
	QString url;
	int size;
};

// https://stackoverflow.com/questions/27400025/using-qabstractlistmodel-in-listview

class MapDataModel : public QAbstractListModel
{
	Q_OBJECT
public:
	MapDataModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;

protected:
	QHash<int, QByteArray> roleNames() const override;

private:
	std::vector<MapData> m_mapData;
};