#include "Avatar.h"

using namespace std;

// AVATAR SECTION
Avatar::Avatar(ef::Vector3 position, ef::Vector3 angles): m_head(angles), m_position(position) {

}

const ef::Vector3& Avatar::getHeadAngles() const {
	return m_head.getAngles();
}

void Avatar::rotateHead(const ef::Vector3& mods) {
	m_head.rotate(mods);
}

const ef::Vector3& Avatar::getPosition() const {
	return m_position;
}

void Avatar::move(const ef::Vector3& mods) {
	m_position += mods;
}

void Avatar::moveTo(const ef::Vector3& newPos) {
	m_position = newPos;
}

// HEAD SECTION
Avatar::Head::Head(ef::Vector3 angles): m_angles(angles) {

}

void Avatar::Head::rotate(const ef::Vector3& mods) {
	m_angles += mods;
}

const ef::Vector3& Avatar::Head::getAngles() const {
	return m_angles;
}