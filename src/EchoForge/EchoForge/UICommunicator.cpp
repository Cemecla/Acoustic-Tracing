#include <cctype>
#include <iostream>
#include <QDebug>

#include "UICommunicator.h"
#include "EFView.h"

UICommunicator::UICommunicator(EFView* view): m_parent(view), m_pos(ef::Vector3F()), m_angles(ef::Vector3F()) {}

UICommunicator::UICommunicator(EFView* view, ef::Vector3F pos, ef::Vector3F angles) : m_parent(view), m_pos(pos), m_angles(angles) {}

void UICommunicator::setAvatarPos(ef::Vector3F& v) {
	m_pos = v;
	emit avatarPosChanged();
}

void UICommunicator::setHeadAngles(ef::Vector3F& v) {
	m_angles = v;
	emit headAnglesChanged();
}

const ef::Vector3F& UICommunicator::avatarPos() const {
	return m_pos;
}

const ef::Vector3F& UICommunicator::headAngles() const {
	return m_angles;
}

void UICommunicator::startSimulation() {
	m_parent->startSimulation();
}

void UICommunicator::stopSimulation() {

}

void UICommunicator::saveSimulation(const QString& url) {

}

void UICommunicator::loadSimulation(const QString& url) {
	std::string urlconverted = url.toStdString();
	m_parent->loadSimulation(urlconverted);
}

void UICommunicator::addBlock(const int x, const int y, const int z) {

}

void UICommunicator::destroyBlock(const int x, const int y, const int z) {

}

void UICommunicator::testing(const QString& value) {
	qDebug() << value;
}

void UICommunicator::playtest(const QString& url) {
	std::string urlconverted = url.toStdString();
	m_parent->play(urlconverted);
}

void UICommunicator::userInput(char input) {
	input = std::tolower(input);

	switch (input)
	{
		case 'a':
			break;
		case 'd':
			break;
		default:
			break;
	}
}