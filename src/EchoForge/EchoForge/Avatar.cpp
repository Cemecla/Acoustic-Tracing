#include "Avatar.h"

using namespace std;

// AVATAR SECTION
Avatar::Avatar(EF::Vector3 position, EF::Vector3 angles): m_head(angles), m_position(position) {

}

const EF::Vector3& Avatar::getHeadAngles() const {
	return m_head.getAngles();
}

void Avatar::rotateHead(const EF::Vector3& mods) {
	m_head.rotate(mods);
}

const EF::Vector3& Avatar::getPosition() const {
	return m_position;
}

void Avatar::move(const EF::Vector3& mods) {
	m_position += mods;
}

void Avatar::moveTo(const EF::Vector3& newPos) {
	m_position = newPos;
}

// HEAD SECTION
Avatar::Head::Head(EF::Vector3 angles): m_angles(angles) {

}

void Avatar::Head::rotate(const EF::Vector3& mods) {
	m_angles += mods;
}

const EF::Vector3& Avatar::Head::getAngles() const {
	return m_angles;
}