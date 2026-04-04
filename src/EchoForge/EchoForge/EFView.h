#pragma once

#include <string>

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
			void setupQGuiApplication(QGuiApplication& app) const;
		
		private:
			EFController* m_parent;
			QQmlApplicationEngine* m_qtEngine;
			UICommunicator* m_uiCommunicator;
			void setupCommunicator();
	};

#endif