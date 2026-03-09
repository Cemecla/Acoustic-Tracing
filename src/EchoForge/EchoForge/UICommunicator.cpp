#include <cctype>

#include "UICommunicator.h"
#include "EFView.h"

UICommunicator::UICommunicator(EFView* view): m_parent(view), m_pos(ef::Vector3()), m_angles(ef::Vector3()) {}

UICommunicator::UICommunicator(EFView* view, ef::Vector3 pos, ef::Vector3 angles) : m_parent(view), m_pos(pos), m_angles(angles) {}

void UICommunicator::setAvatarPos(ef::Vector3& v) {
	m_pos = v;
	emit avatarPosChanged();
}

void UICommunicator::setHeadAngles(ef::Vector3& v) {
	m_angles = v;
	emit headAnglesChanged();
}

const ef::Vector3& UICommunicator::avatarPos() const {
	return m_pos;
}

const ef::Vector3& UICommunicator::headAngles() const {
	return m_angles;
}

void UICommunicator::startSimulation() {
	m_parent->startSimulation();
}

void UICommunicator::stopSimulation() {

}

void UICommunicator::saveSimulation(std::string url) {

}

void UICommunicator::loadSimulation(std::string url) {

}

void UICommunicator::addBlock(const int x, const int y, const int z) {

}

void UICommunicator::destroyBlock(const int x, const int y, const int z) {

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