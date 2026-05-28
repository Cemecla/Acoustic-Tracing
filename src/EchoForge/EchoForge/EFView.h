#pragma once

#include "Global.h"

#include "JuceHeader.h"
#include "AudioManager.h"
#include "SoundRemaining.h"

#include "Map.h"
#include "Avatar.h"
#include "Utils.h"
#include "Vector.h"
#include "EFWorld.h"

class EFController;
class QQmlApplicationEngine;
class QGuiApplication;
class UICommunicator;

#ifndef EF_VIEW_H
	#define EF_VIEW_H
	class EFView {
		public:
			EFView(EFController* parent);

			void update(float delta);
			void startSimulation();
			void saveSimulation();
			void stopSimulation();
			void pauseSimulation();
			void saveSimulation(std::string& url);
			void loadSimulation(std::string& url);

			json loadPinnaSetting();
			void savePinnaSetting();

			void setSelectedPinnaId(int id);
			int getSelectedPinnaId() const;

			void addSound(const std::string& path) const;
			void addAmbient(const std::string& path) const;

			int playSound(int soundFileId) const;
			const std::vector<yutasound::AmbientEntry>& getAmbientEntries() const;

			int  registerSound(const std::string& path) const;
			int  playSound(int soundFileId, float azimuth, float elevation, ef::Vector3F gain) const;
			void updateSoundSource(int emitterId, float azimuth, float elevation, ef::Vector3F gain) const; // this is for the senarios and scene 
			void stopAudio() const;

			void resetEmitters() const;
			void resumeAll() const;
			void pauseAll() const;
			void resumeEmitter(int emitterId) const;
			void loopEmitter(int emitterId) const;
			void pauseEmitter(int emitterId) const;
			void stopEmitter(int emitterId) const;

			const ef::Map& getMap() const;
			QVariantList getSounds() const;
			void loadRegistry(const std::string& path) const;
			void saveRegistry(const std::string& path) const;
			void removeSound(int id) const;
			void unloadRegistry() const;

			void setupQGuiApplication(QGuiApplication& app) const;
			void setupAudioManager(yutasound::AudioManager* audioManager);
			void setupAvatar(const ef::Avatar& avatar);
			void updateSoundSource(int emitterId, float azimuth, float elevation) const;
			void buildMap(const ef::Map& map);
			void simulationLoaded() const;
			const Inputs& getLastInput() const;

			void updateRayInstance();

			void setEmittersVisual(const std::vector<ef::Emitter>& emitters);
		
			
			const float getRefreshTime() const;
			void setRefreshTime(float timeInSeconde);

			const int getMaxRayCount() const;
			void setMaxRayCount(int rayCount);

			const int getRayPerFrame() const;
			void setRayPerFrame(int rayPerFrame);
		
		private:
			EFController* m_parent;
			QQmlApplicationEngine* m_qtEngine;
			yutasound::AudioManager* m_audioManager = nullptr;
			UICommunicator* m_uiCommunicator;
			int m_selectedPinnaId = 24;
	};

#endif