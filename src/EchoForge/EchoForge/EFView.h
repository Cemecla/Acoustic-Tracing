#pragma once
class EFController;
class QQmlApplicationEngine;

#ifndef __EFView__
	#define __EFView__
	class EFView
	{
		public:
			EFView(EFController* parent);
			~EFView();
		
		private:
			EFController* m_parent;
			QQmlApplicationEngine* m_qtEngine;
	};

#endif