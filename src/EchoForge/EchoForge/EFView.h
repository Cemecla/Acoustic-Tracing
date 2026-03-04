#pragma once
class EFController;
class QQmlApplicationEngine;

#ifndef EF_VIEW_H
	#define EF_VIEW_H
	class EFView {
		public:
			EFView(EFController* parent);

			void update();
		
		private:
			EFController* m_parent;
			QQmlApplicationEngine* m_qtEngine;
	};

#endif