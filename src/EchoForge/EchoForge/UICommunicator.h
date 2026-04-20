#pragma once
#include <QObject>
#include <string>
#include <QVector3D>
#include <QVariant>

#include "Vector.h"
#include "Map.h"
#include "ChunkInstancing.h"
#include "RayInstancing.h"
#include "Avatar.h"

class EFView;
class DTO_RecievedRay;

/// <summary>
/// Testing
/// https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html
/// </summary>
class UICommunicator: public QObject {
	Q_OBJECT
	public:
		UICommunicator(EFView* view);
		UICommunicator(EFView* view, ef::Vector3F pos, ef::Vector3F angles);
		// READ -> Getter WRITE -> Setter NOTIFY -> SIGNAL
		Q_PROPERTY(QVector3D avatarPos READ avatarPos WRITE setAvatarPos NOTIFY avatarPosChanged);
		Q_PROPERTY(QVector3D headAngles READ headAngles WRITE setHeadAngles NOTIFY headAnglesChanged);
		Q_PROPERTY(ChunkInstancing* voxelInstances READ voxelInstances NOTIFY chunkInstancingsChanged);
		Q_PROPERTY(RayInstancing* rayInstances READ rayInstances NOTIFY rayInstancingsChanged);

		QVector3D avatarPos() const;
		QVector3D headAngles() const;
		ChunkInstancing* voxelInstances();
		RayInstancing* rayInstances();

		void setAvatarPos(QVector3D& v);
		void setHeadAngles(QVector3D& v);

		Q_INVOKABLE void startSimulation();
		Q_INVOKABLE void stopSimulation();
		Q_INVOKABLE void saveSimulation(const QString& url);
		Q_INVOKABLE void loadSimulation(const QString& url);
		Q_INVOKABLE void userInput(char input);
		Q_INVOKABLE void addBlock(int x, int y, int z);
		Q_INVOKABLE void destroyBlock(int x, int y, int z);

		Q_INVOKABLE void testing(const QString& value);
		Q_INVOKABLE void playtest(const QString& url);
		Q_INVOKABLE void stoptest(const QString& url);
		Q_INVOKABLE void updateTestSlider(const float azimuth, const float elevation);

		void buildMap(const ef::Map& map);
		void updateRayInstances(const DTO_RecievedRay& receivedRays);
		void setupAvatar(const ef::Avatar& avatar);
		void callSimulationLoaded();

	signals:
		void avatarPosChanged();
		void headAnglesChanged();
		void chunkInstancingsChanged();
		void simulationLoaded();
		void rayInstancingsChanged();


	private:
		QVector3D m_pos;
		QVector3D m_angles;
		ChunkInstancing* m_voxelInstances;
		RayInstancing* m_rayInstances;
		EFView* m_parent;
};