#pragma once
class EFController;
class QQmlApplicationEngine;
class UICommunicator;

#ifndef EF_VIEW_H
	#define EF_VIEW_H
	class EFView {
		public:
			EFView(EFController* parent);

			void update();
			void startSimulation();
			void saveSimulation();
			void stopSimulation();
			void saveSimulation(std::string url);
			void loadSimulation(std::string url);
		
		private:
			EFController* m_parent;
			QQmlApplicationEngine* m_qtEngine;
			UICommunicator* m_uiCommunicator;

			void setupCommunicator();
	};

#endif