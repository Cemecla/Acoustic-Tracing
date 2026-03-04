#include "Avatar.h"

using namespace std;

// AVATAR SECTION
Avatar::Avatar(Vector::Vector3 position, Vector::Vector3 angles): m_head(angles), m_position(position) {

}

const Vector::Vector3& Avatar::getHeadAngles() const {
	return m_head.getAngles();
}

void Avatar::rotateHead(const Vector::Vector3& mods) {
	m_head.rotate(mods);
}

const Vector::Vector3& Avatar::getPosition() const {
	return m_position;
}

void Avatar::move(const Vector::Vector3& mods) {
	m_position += mods;
}

void Avatar::moveTo(const Vector::Vector3& newPos) {
	m_position = newPos;
}

// HEAD SECTION
Avatar::Head::Head(Vector::Vector3 angles): m_angles(angles) {

}

void Avatar::Head::rotate(const Vector::Vector3& mods) {
	m_angles += mods;
}

const Vector::Vector3& Avatar::Head::getAngles() const {
	return m_angles;
}