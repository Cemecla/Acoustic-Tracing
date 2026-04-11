#include "SimulationLoop.h"

// https://stackoverflow.com/questions/15236340/how-i-can-pass-callable-object-to-function-as-parameter
SimulationLoop::SimulationLoop(QObject* parent, int intervalMsec): QObject(parent), m_timer(QTimer(this)), m_elapsedTimer(QElapsedTimer()) {
	init(intervalMsec);
}

void SimulationLoop::setCallback(std::function<void(float)> callback) {
	m_callback = callback;
}

void SimulationLoop::stop() {
	m_timer.stop();
}

void SimulationLoop::start() {
	if (m_elapsedTimer.isValid()) { m_elapsedTimer.restart(); }
	else { m_elapsedTimer.start(); }

	m_timer.start();
}

void SimulationLoop::update() {
	// Restart return in miliseconde / 1000 to change in secondes
	float delta = (float)m_elapsedTimer.restart() / 1000;

	if (m_callback)
		m_callback(delta);
}

void SimulationLoop::init(int intervalMsec) {
	m_timer.setInterval(intervalMsec);
	connect(&m_timer, &QTimer::timeout, this, &SimulationLoop::update);
}