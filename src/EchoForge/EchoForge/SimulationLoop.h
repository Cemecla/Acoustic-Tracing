#pragma once
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>



class SimulationLoop: public QObject {
	Q_OBJECT

public:
	SimulationLoop(QObject* parent = nullptr, int intervalMsec = 16);

	void setCallback(std::function<void(float)>);

	void stop();

	void start();

private:
	QTimer m_timer;
	QElapsedTimer m_elapsedTimer;
	std::function<void(float)> m_callback;

	void update();
	void init(int intervalMsec);

};