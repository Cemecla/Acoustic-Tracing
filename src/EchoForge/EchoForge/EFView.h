#pragma once
class EFController;
class QQmlApplicationEngine;

class EFView
{
	public:
		EFView(EFController* parent);
		~EFView();
	
	private:
		EFController* m_parent;
		QQmlApplicationEngine* m_qtEngine;
};