#pragma once

#include <string>

#include "JuceHeader.h"
#include "AudioManager.h"
#include "SoundRemaining.h" 

#include "Map.h"

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
			void play(std::string& url) const;
			void stop(std::string& url) const;
			void setupQGuiApplication(QGuiApplication& app) const;
			void setupAudioManager(yutasound::AudioManager* audioManager);
			void updateListener(const float azimuth, const float elevation) const;
			void buildMap(ef::Map& map);
			void simulationLoaded() const;
		
		private:
			EFController* m_parent;
			QQmlApplicationEngine* m_qtEngine;
			yutasound::AudioManager* m_audioManager = nullptr;
			UICommunicator* m_uiCommunicator;
			void setupCommunicator();
	};

#endif