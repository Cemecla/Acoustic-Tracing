#pragma once

#include <string>

#include "JuceHeader.h"
#include "AudioManager.h"
#include "SoundRemaining.h"

#include "Map.h"
#include "Avatar.h"
#include "Utils.h"

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
			void saveSimulation(std::string& url);
			void loadSimulation(std::string& url);

			void addSound(const std::string& path) const;
			int playFromFileId(int soundFileId) const;
			void play(std::string& url) const;
			void stop(std::string& url) const;
			void pausetest() const;
			void resumetest() const;
			void resetEmitters() const;
			void resumeAll() const;
			void pauseAll() const;
			void resumeEmitter(int emitterId) const;
			void pauseEmitter(int emitterId) const;
			void stopEmitter(int emitterId) const;
			QVariantList getSounds() const;
			void loadRegistry(const std::string& path) const;
			void saveRegistry(const std::string& path) const;
			void removeSound(int id) const;
			void unloadRegistry() const;
			void setupQGuiApplication(QGuiApplication& app) const;
			void setupAudioManager(yutasound::AudioManager* audioManager);
			void setupAvatar(const ef::Avatar& avatar);
			void updateListener(int emitterId, float azimuth, float elevation) const;
			void buildMap(const ef::Map& map);
			void simulationLoaded() const;
			const Inputs& getLastInput() const;

			void updateRayInstance();
		
		private:
			int m_testEmitterId = -1;
			EFController* m_parent;
			QQmlApplicationEngine* m_qtEngine;
			yutasound::AudioManager* m_audioManager = nullptr;
			UICommunicator* m_uiCommunicator;
	};

#endif