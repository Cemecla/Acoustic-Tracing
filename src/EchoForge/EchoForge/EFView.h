#pragma once
class EFController;
class QQmlApplicationEngine;

class EFView
{
	public:
		EFView(EFController* parent);
		~EFView();
	
	private:
		EFController* parent_;
		QQmlApplicationEngine* qtEngine_;
};