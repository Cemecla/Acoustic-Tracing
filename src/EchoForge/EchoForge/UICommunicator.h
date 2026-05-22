#pragma once
#include "Global.h"

#include <QObject>
#include <QVector3D>
#include <QVariant>

#include "Vector.h"
#include "Map.h"
#include "Avatar.h"

#include "ChunkInstancing.h"
#include "RayInstancing.h"

class EFView;
class DTO_RecievedRay;

/// <summary>
/// Documentation
/// https://doc.qt.io/qt-6/qtqml-cppintegration-exposecppattributes.html
/// https://ekkesapps.wordpress.com/qt-6-qmake/qmetatype/
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
		Q_INVOKABLE void userInput(int key, bool pressed);
		Q_INVOKABLE void addBlock(int x, int y, int z);
		Q_INVOKABLE void destroyBlock(int x, int y, int z);

		// audio testing page stuff 

		Q_INVOKABLE void resetEmitters();
		Q_INVOKABLE void resumeAll();
		Q_INVOKABLE void pauseAll();

		Q_INVOKABLE void updateTestSlider(int emitterId, float azimuth, float elevation);
		Q_INVOKABLE int playFromFileId(int soundFileId);
		
		Q_INVOKABLE void resumeEmitter(int emitterId);
		Q_INVOKABLE void loopEmitter(int emitterId);
		Q_INVOKABLE void pauseEmitter(int emitterId);
		Q_INVOKABLE void stopEmitter(int emitterId);
		

		Q_INVOKABLE void addSound(const QString& path);
		Q_INVOKABLE QVariantList getSounds();

		Q_INVOKABLE void loadRegistry(const QString& path);
		Q_INVOKABLE void saveRegistry(const QString& path);
		Q_INVOKABLE void removeSound(int id);
		Q_INVOKABLE void unloadRegistry();

		Q_INVOKABLE QVariantList loadPinnaSetting();
		Q_INVOKABLE void savePinnaSetting(QVariantMap& settings);

		Q_INVOKABLE void setSelectedPinnaId(int id);

		//fin

		void buildMap(const ef::Map& map);
		void updateRayInstances(const std::vector<DTO_RecievedRay>& rays);
		void setupAvatar(const ef::Avatar& avatar);
		void callSimulationLoaded();
		const Inputs& getInputs() const;

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

		Inputs m_inputs{};
};